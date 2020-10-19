#ifndef SQLCONNRAII__H
#define SQLCONNRAII__H
#include "sqlconnpool.h"


class sqlconnRAII{
public:
    sqlconnRAII(MYSQL** sql,ConnectionPool* connpool)
    {
        assert(connpool);
        *sql=connpool->GetConnection();
        sql_=*sql;
        connpool_=connpool;
    }
    ~sqlconnRAII()
    {
        if(sql_)
            connpool_->ReleaseConnection(sql_);//放回连接池
    }
private:
    MYSQL * sql_;
    ConnectionPool *connpool_;
};

#endif