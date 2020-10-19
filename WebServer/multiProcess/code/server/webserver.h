#ifndef WEBSERVER__H
#define WEBSERVER__H
#include "../cpplang.h"
#include "epoller.h"
#include "../log/log.h"
#include "../timer/heaptimer.h"
#include "../http/httpconn.h"
#include "../pool/sqlconnRAII.h"
#include "../pool/processpool.h"
#include "../pool/sqlconnpool.h"

class WebServer{
public:
    WebServer
    (
        int port_,int timeoutMS_,bool openLinger_,
        int sqlPort,const char* sqlUser,const char* sqlPwd,
        const char* dbName,int connPoolNum,int ProcessNum,
        bool openLog,int logLevel,int logQueSize
    );
    ~WebServer();
    void Start();
private:
    bool InitSocket();

private:
    int port;
    int openLinger;
    int timeoutMS;
    int listenFd;
    char* srcDir;

    std::unique_ptr<processpool> threadpool;
    std::unordered_map<int,HttpConn> users;
};




#endif