#include "webserver.h"


WebServer::WebServer(int port_,int trigMode_,int timeoutMS_,bool openLinger_,
                    int sqlPort,const char* sqlUser,const char* sqlPwd,
                    const char* dbName,int connPoolNum,int threadNum,
                    bool openLog,int logLevel,int logQueSize):
                    port(port_),openLinger(openLinger_),timeoutMS(timeoutMS_),isClose(false),
                    timer(new HeapTimer()),threadpool(new ThreadPool(threadNum)),epoller(new Epoller())
{
    srcDir=getcwd(nullptr,256);//malloc动态分配内存
    assert(srcDir);
    strncat(srcDir,"/resources",16);
    HttpConn::userCount=0;
    HttpConn::srcDir=srcDir;
    ConnectionPool::GetInstance()->init("localhost",sqlUser,sqlPwd,dbName,sqlPort,connPoolNum);
    
    InitEventModel(trigMode_);
    if(!InitSocket()) isClose=true;
    if(openLog)
    {
        Log::Instance()->init(logLevel,"./log",".log",logQueSize);
        if(isClose)
        {
            LOG_ERROR("=============Server init error=============");
        }
        else
        {
            LOG_INFO("=============Server init=============");
            LOG_INFO("Port:%d,OpenLinger:%s",port,openLinger?"true":"false");
            LOG_INFO("Listen Mode:%s,Conn Mode:%s",
                    (listenEvent&EPOLLET?"ET":"LT"),
                    (connEvent&EPOLLET?"ET":"LT"));
            LOG_INFO("LogSys level:%d",logLevel);
            LOG_INFO("srcDir:%s",HttpConn::srcDir);
            LOG_INFO("SqlConnPool num:%d,ThreadPool num:%d",connPoolNum,threadNum);
        }
    }
}
WebServer::~WebServer()
{
    close(listenFd);
    isClose=true;
    free(srcDir);
    ConnectionPool::GetInstance()->DestoryConn();
}
void WebServer::Start()
{
    int timeMS=-1;
    if(!isClose) 
        LOG_INFO("=============Server Start=============");
    while(!isClose)
    {
        if(timeoutMS>0)
        {
            timeMS=timer->GetNextTick();
        }
        int eventCnt=epoller->wait(timeMS);
        for(int i=0;i<eventCnt;i++)
        {
            int fd=epoller->GetEventFd(i);
            uint32_t events=epoller->GetEvents(i);
            if(fd==listenFd)
            {
                DealListen();
            }
            else if(events&(EPOLLRDHUP|EPOLLHUP|EPOLLERR))//error
            {
                assert(users.count(fd)>0);                
                CloseConn(&users[fd]);
            }
            else if(events&EPOLLIN)
            {
                assert(users.count(fd)>0);
                DealRead(&users[fd]);
            }
            else if(events&EPOLLOUT)
            {
                assert(users.count(fd)>0);
                DealWrite(&users[fd]);
            }
            else
            {
                LOG_ERROR("Unexpected event");
            }
        }
    }
}
bool WebServer::InitSocket()
{
    int ret;
    struct sockaddr_in addr;
    if(port>65535||port<1024)
    {
        LOG_ERROR("Port:%d error!",port);
        return false;
    }
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(port);
    struct linger optlinger={0};
    if(openLinger)
    {
        optlinger.l_onoff=1;
        optlinger.l_linger=1;
    }
    listenFd=socket(AF_INET,SOCK_STREAM,0);
    if(listenFd<0)
    {
        LOG_ERROR("Create socket error!");
        return false;
    }
    ret=setsockopt(listenFd,SOL_SOCKET,SO_LINGER,&optlinger,sizeof(optlinger));
    if(ret<0)
    {
        close(listenFd);
        LOG_ERROR("Init Linger error!");
        return false;
    }
    int optval=1;
    ret=setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(int));
    if(ret<0)
    {
        close(listenFd);
        LOG_ERROR("Set Socket error!");
        return false;
    }
    ret=bind(listenFd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret<0)
    {
        close(listenFd);
        LOG_ERROR("Bind Port:%d error!",port);
        return false;
    }
    ret=listen(listenFd,6);
    if(ret<0)
    {
        close(listenFd);
        LOG_ERROR("Listen Port:%d error!",port);
        return false;        
    }
    ret=epoller->AddFd(listenFd,listenEvent|EPOLLIN);
    if(ret==0)
    {
        close(listenFd);
        LOG_ERROR("Add listenFd error!");
        return false;
    }
    SetFdNoBlock(listenFd);
    LOG_INFO("Server port:%d",port);
    return true;
}
void WebServer::InitEventModel(int trigMode)
{
    listenEvent=EPOLLRDHUP;
    connEvent=EPOLLONESHOT|EPOLLRDHUP;
    switch(trigMode)
    {
        case 0://LT+LT
            break;
        case 1://LT+ET
            connEvent|=EPOLLET;
            break;
        case 2:
            listenEvent|=EPOLLET;//ET+LT
            break;
        case 3:
            listenEvent|=EPOLLET;//ET+ET
            connEvent|=EPOLLET;
            break;
        default:
            break;
    }
    HttpConn::isET=(connEvent&EPOLLET);
}
void WebServer::AddClient(int fd,sockaddr_in addr)
{
    assert(fd>0);
    users[fd].init(fd,addr);//还是要理解
    if(timeoutMS>0)
    {
        timer->add(fd,timeoutMS,std::bind(&WebServer::CloseConn,this,&users[fd]));
    }
    epoller->AddFd(fd,connEvent|EPOLLIN);
    SetFdNoBlock(fd);
    LOG_INFO("Client[%d] is Added in timer",users[fd].GetFd());

}
void WebServer::DealListen()
{
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    do
    {
        int fd=accept(listenFd,(struct sockaddr*)&addr,&len);
        if(fd<=0)
            break;
        else if(HttpConn::userCount>=MAX_FD)
        {
            SendError(fd,"Server busy!");
            LOG_WARN("Client is full!");
            return;
        }
        AddClient(fd,addr);
    }while(listenEvent&EPOLLET);
}
void WebServer::DealWrite(HttpConn* client)
{
    assert(client);
    ExtentTime(client);
    threadpool->AddTask(std::bind(&WebServer::OnWrite,this,client));
}
void WebServer::DealRead(HttpConn* client)
{
    assert(client);
    ExtentTime(client);
    threadpool->AddTask(std::bind(&WebServer::OnRead,this,client));
}
void WebServer::SendError(int fd,const char* info)
{
    assert(fd>0);
    int ret=send(fd,info,strlen(info),0);//
    if(ret<0)
        LOG_WARN("Send error to client[%d] error!",fd);
    close(fd);
}
void WebServer::ExtentTime(HttpConn* client)
{
    assert(client);
    if(timeoutMS>0)
    {
        timer->adjust(client->GetFd(),timeoutMS);
    }
}
void WebServer::CloseConn(HttpConn* client)
{
    assert(client);
    LOG_INFO("Client[%d] quit!",client->GetFd());
    epoller->DelFd(client->GetFd());
    client->Close();
}
void WebServer::OnRead(HttpConn* client)
{
    assert(client);
    int ret=-1;
    int readErrno=0;
    ret=client->read(&readErrno);
    if(ret<=0&&readErrno!=EAGAIN)
    {
        CloseConn(client);
        return;
    }
    client->process();
    epoller->ModFd(client->GetFd(),connEvent|EPOLLOUT);
}
void WebServer::OnWrite(HttpConn* client)
{
    assert(client);
    int ret=-1;
    int writeErrno=0;
    ret=client->write(&writeErrno);
    if(client->ToWriteByte()==0)
    {
        if(client->IsKeepAlive())
        {
            LOG_DEBUG("Keep-Alive!");
            client->reset();
            epoller->ModFd(client->GetFd(),connEvent|EPOLLIN);
            return ;
        }
    }
    else if(ret<0)
    {
        if(writeErrno==EAGAIN)
        {
            epoller->ModFd(client->GetFd(),connEvent|EPOLLOUT);
            return;
        }
    }
    CloseConn(client);
}
int WebServer::SetFdNoBlock(int fd)
{
    assert(fd > 0);
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    return fcntl(fd,F_SETFL,new_option);
}
