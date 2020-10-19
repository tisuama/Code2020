#ifndef EPOLL__H
#define EPOLL__H

#include "../cpplang.h"

class Epoller{
public:
    explicit Epoller(int maxEvent=1024);
    ~Epoller();
    bool AddFd(int fd,uint32_t events);
    bool ModFd(int fd,uint32_t events);
    bool DelFd(int fd);
    int wait(int timeouts=-1);
    int GetEventFd(size_t i) const;
    uint32_t GetEvents(size_t i) const;
    int GET_EPOLL_Fd()const;
private:
    int epollFd;
    std::vector<struct epoll_event> All_events;
};


#endif