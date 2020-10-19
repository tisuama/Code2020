#include "httpconn.h"

bool HttpConn::isET;//默认值
std::atomic<int> HttpConn::userCount{0};
const char* HttpConn::srcDir="";


HttpConn::HttpConn()
{
    fd=-1;
    addr={0};
    isClose=true;
}
HttpConn::~HttpConn()
{
    Close();
}
void HttpConn::init(int sockFd_,const sockaddr_in& addr_)
{
    assert(sockFd_>0);
    userCount++;
    addr=addr_;
    fd=sockFd_;
    reset();
    LOG_INFO("Client[%d] (%s : %d) IN, userCount: %d",fd,GetIp(),GetPort(),(int)userCount);
}
void HttpConn::reset()
{
    writeBuffer.RetrieveAll();
    readBuffer.RetrieveAll();
    isClose=false;
    request.Init();
}
ssize_t HttpConn::read(int* saveErrno)
{
    ssize_t len=-1;
    do
    {
        len=readBuffer.readFd(fd,saveErrno);
        if(len<=0)
            break;
    }while(isET);//一次读完
    return len;
}
ssize_t HttpConn::write(int* saveErrno)//buff的写比较简单
{
    ssize_t len=-1;
    do
    {
        len=writev(fd,iov,iovCnt);
        if(len<=0)
        {
            *saveErrno=errno;
            break;
        }
        if(iov[0].iov_len+iov[1].iov_len==0)//传输结束
            break;
        else if(static_cast<size_t>(len)>iov[0].iov_len)
        {
            iov[1].iov_base=(uint8_t*)(iov[1].iov_base+(len-iov[0].iov_len));
            iov[1].iov_len-=(len-iov[0].iov_len);
            if(iov[1].iov_len)
            {
                writeBuffer.RetrieveAll();//这里表示第一部分读完了 即writeBuffer空
                iov[0].iov_len=0;
            }
        }
        else
        {
            iov[0].iov_base=(uint8_t*)(iov[0].iov_base+len);
            iov[0].iov_len-=len;
            writeBuffer.Retrieve(len);
        }
    } while(isET||ToWriteByte()>10240);//10240 优化？？存储很多就可以发送了？？
    return len;
    
}
void HttpConn::Close()
{
    response.unmapFile();
    if(!isClose)
    {
        isClose=true;
        userCount--;
        close(fd);
        LOG_INFO("Client[%d] (%s : %d) QUIT, userCount: %d",fd,GetIp(),GetPort(),(int)userCount)
    }
}
int HttpConn::GetFd() const
{
    return fd;
}
int HttpConn::GetPort() const
{
    return ntohs(addr.sin_port);//htons
}
const char* HttpConn::GetIp() const
{
    return inet_ntoa(addr.sin_addr);//htol/inet_addr
}
sockaddr_in HttpConn::GetAddr() const
{
    return addr;
}
void HttpConn::process()
{
    //根据request情况初始化response
    if(request.parse(readBuffer))
    {
        LOG_DEBUG("%s",request.path().data());
        response.Init(srcDir,request.path(),request.IsKeepAlive(),200);
    }
    else
        response.Init(srcDir,request.path(),false,400);
    response.MakeResponse(writeBuffer);
    iov[0].iov_base=const_cast<char*>(writeBuffer.Peek());//去掉const 常量指针不能赋给非常量指针
    iov[0].iov_len=writeBuffer.ReadableBytes();
    iovCnt=1;
    if(response.FileLen()>0&&response.File())
    {
        iov[1].iov_base=response.File();
        iov[1].iov_len=response.FileLen();
        iovCnt=2;
    }
    LOG_DEBUG("FileSize:%d, %d to %d",response.FileLen(),iovCnt,ToWriteByte());
}
