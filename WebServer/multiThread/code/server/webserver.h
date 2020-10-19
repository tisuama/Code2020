#ifndef WEBSERVER__H
#define WEBSERVER__H
#include "../cpplang.h"
#include "epoller.h"
#include "../log/log.h"
#include "../timer/heaptimer.h"
#include "../http/httpconn.h"
#include "../pool/sqlconnRAII.h"
#include "../pool/threadpool.h"
#include "../pool/sqlconnpool.h"

class WebServer{
public:
    WebServer
    (
        int port_,int trigMode_,int timeoutMS_,bool OptLiner_,
        int sqlPort,const char* sqlUser,const char* sqlPwd,
        const char* dbName,int connPoolNum,int threadNum,
        bool openLog,int logLevel,int logQueSize
    );
    ~WebServer();
    void Start();
private:
    bool InitSocket();
    void InitEventModel(int trigMode);
    void AddClient(int fd,sockaddr_in addr);

    void DealListen();
    void DealWrite(HttpConn* client);
    void DealRead(HttpConn* client);
    void SendError(int fd,const char* info);
    void ExtentTime(HttpConn* client);
    void CloseConn(HttpConn* client);

    void OnRead(HttpConn* client);
    void OnWrite(HttpConn* client);
    static int SetFdNoBlock(int fd);

private:
    static const int MAX_FD=65536;
    int port;
    int openLinger;
    int timeoutMS;
    int isClose;
    int listenFd;
    char* srcDir;

    uint32_t listenEvent;
    uint32_t connEvent;

    std::unique_ptr<HeapTimer> timer;
    std::unique_ptr<ThreadPool> threadpool;
    std::unique_ptr<Epoller> epoller;
    std::unordered_map<int,HttpConn> users;
};




#endif