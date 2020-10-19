#include "webserver.h"


WebServer::WebServer(int port_,int timeoutMS_,bool openLinger_,
                    int sqlPort,const char* sqlUser,const char* sqlPwd,
                    const char* dbName,int connPoolNum,int ProcessNum,
                    bool openLog,int logLevel,int logQueSize):
                    port(port_),openLinger(openLinger_),timeoutMS(timeoutMS_)
{
    srcDir=getcwd(nullptr,256);//malloc动态分配内存
    assert(srcDir);
    strncat(srcDir,"/resources",16);
    HttpConn::userCount=0;
    HttpConn::srcDir=srcDir;
    ConnectionPool::GetInstance()->init("localhost",sqlUser,sqlPwd,dbName,sqlPort,connPoolNum);
    
    InitSocket();
    if(openLog)
    {
        Log::Instance()->init(logLevel,"./log",".log",logQueSize);
    }
}


void WebServer::Start()
{
    processpool::Instance()->init(listenFd);
    processpool::Instance()->run();
}

WebServer::~WebServer()
{
    close(listenFd);
    free(srcDir);
    ConnectionPool::GetInstance()->DestoryConn();
}


static int SetFdNoBlock(int fd)
{
    assert(fd > 0);
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    return fcntl(fd,F_SETFL,new_option);
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
    SetFdNoBlock(listenFd);
    LOG_INFO("Server port:%d",port);
    return true;
}

