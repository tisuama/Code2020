#ifndef HTTPCONN__H
#define HTTPCONN__H

#include "../cpplang.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "../log/log.h"

class HttpConn{
public:
    HttpConn();
    ~HttpConn();

    void init(int sockFd_,const sockaddr_in& addr_);
    void reset();
    ssize_t read(int* saveErrno);
    ssize_t write(int* saveErrno);

    void Close();
    int GetFd() const;
    int GetPort() const;
    const char* GetIp() const;
    sockaddr_in GetAddr() const;
    
    void process();
    int ToWriteByte()
    {
        return iov[0].iov_len+iov[1].iov_len;
    }
    bool IsKeepAlive() const
    {
        return request.IsKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;

private:
    int fd;
    struct sockaddr_in addr;

    bool isClose;
    int iovCnt;
    struct iovec iov[2];
    
    Buff readBuffer;
    Buff writeBuffer;

    HttpRequest request;
    HttpResponse   response;
};



#endif