#ifndef PROCESSPOOL__H
#define PROCESSPOOL__H

#include "../cpplang.h"
#include "../http/httpconn.h"
#include "../log/log.h"

class process//子进程
{
public:
    process():m_pid(-1){}

public:
    pid_t m_pid;
    int m_pipefd[2];

};

class processpool//线程池
{
private:
    processpool()=default;

public:
    static processpool* Instance();
    void init(int listenfd,int process_number=4);
    void run();
     ~processpool()=default;

private:
    void setup_sig_pipe();
    void run_parent();
    void run_child();
    void OnRead(HttpConn* client);
    void OnWrite(HttpConn* client);
    void CloseConn(HttpConn* client);


    
private:
    static const int MAX_PROCESS_NUMBER=8;
    static const int MAX_EVENT_NUMBER=1024;
    struct pool
    {
        int m_process_num;
        int m_idx;//子进程序号
        int m_epoll;
        int m_listenfd;
        int m_stop;
        process* m_sub_process;
    };
    std::shared_ptr<pool> pool_ptr;
};




#endif