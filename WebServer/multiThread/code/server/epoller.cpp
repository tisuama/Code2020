#include "epoller.h"

Epoller::Epoller(int maxEvent):epollFd(epoll_create(512)),All_events(maxEvent)
{
    assert(epollFd>=0&&All_events.size()>0);
}
Epoller::~Epoller()
{
    close(epollFd);
}
bool Epoller::AddFd(int fd,uint32_t events)
{
    assert(fd>=0);
    struct epoll_event ev={0};
    ev.data.fd=fd;
    ev.events=events;
    return epoll_ctl(epollFd,EPOLL_CTL_ADD,fd,&ev)==0;
}
bool Epoller::ModFd(int fd,uint32_t events)
{
    if(fd<0) return false;
    epoll_event ev={0};
    ev.data.fd=fd;
    ev.events=events;
    return epoll_ctl(epollFd,EPOLL_CTL_MOD,fd,&ev)==0;
}
bool Epoller::DelFd(int fd)
{
    if(fd<0) return false;
    epoll_event ev={0};
    return epoll_ctl(epollFd,EPOLL_CTL_DEL,fd,&ev)==0;
}
int Epoller::wait(int timeouts)
{
    //assert(All_events[0])
    return epoll_wait(epollFd, &All_events[0], static_cast<int>(All_events.size()), timeouts);
}
int Epoller::GetEventFd(size_t i) const
{
    assert(i<All_events.size()&&i>=0);
    return All_events[i].data.fd;
}
uint32_t Epoller::GetEvents(size_t i) const
{
    assert(i<All_events.size()&&i>=0);
    return All_events[i].events;
}