#ifndef SQLCONNPOOL__H
#define SQLCONNPOOL__H

#include "../cpplang.h"
#include<mysql/mysql.h>
#include "../log/log.h"

class ConnectionPool{
public:
    MYSQL* GetConnection();
    void ReleaseConnection(MYSQL *conn);
    int GetFreeConnCount();
    void DestoryConn();

    static ConnectionPool* GetInstance();
    void init(const char* host, 
            const char* user, 
            const char* pwd, 
            const char* databasename, 
            int port, 
            int MaxConn=12);

private:
    ConnectionPool()=default;//单例模式下私有化构造函数
    ~ConnectionPool();

private:
    int m_MaxConn;
    std::mutex mtx;
    std::queue<MYSQL *> connqueue;
    sem_t sem;

};

#endif