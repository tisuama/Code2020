#include "sqlconnpool.h"


ConnectionPool*  ConnectionPool::GetInstance()
{
    static ConnectionPool connPool;
    return &connPool;
}

void ConnectionPool::init(const char* host, const char* user, const char* pwd, const char* databasename, int port, int MaxConn)
{
    assert(MaxConn>0);
    for(int i=1;i<=MaxConn;i++)
    {
        MYSQL *conn=nullptr;
        conn=mysql_init(conn);
        if(!conn)  //LOG
        {
            LOG_ERROR("Mysql init error!");
            assert(conn);
        }
        conn=mysql_real_connect(conn,host,user,pwd,databasename,port,nullptr,0);
        if(!conn)
        {
            LOG_ERROR("Mysql Connect error!");
        }
        connqueue.push(conn);
    }
    MaxConn=connqueue.size();
    sem_init(&sem,0,MaxConn);// 0:当前进程共享
}
//返回了局部指针 使用的时候小心
MYSQL* ConnectionPool::GetConnection()
{
    MYSQL* conn=nullptr;
    if(connqueue.empty())
    {
        LOG_WARN("ConnectionPool busy!");
        return nullptr;
    }
    sem_wait(&sem);//P
    {
        std::lock_guard<std::mutex> lock(mtx);
        conn=connqueue.front(); 
        connqueue.pop();
    } 
    return conn;
}
void ConnectionPool::ReleaseConnection(MYSQL *conn)
{
    assert(conn);
    {
        std::lock_guard<std::mutex> lock(mtx);
        connqueue.push(conn);
    }
    sem_post(&sem);//V
}
int ConnectionPool::GetFreeConnCount()
{
    std::lock_guard<std::mutex> lock(mtx);
    return connqueue.size();
}
void ConnectionPool::DestoryConn()
{
    std::lock_guard<std::mutex> lock(mtx);
    while(!connqueue.empty())
    {
        auto conn=connqueue.front();
        connqueue.pop();
        mysql_close(conn);
    }
    mysql_library_end();
}
ConnectionPool::~ConnectionPool()
{
    DestoryConn();
}
