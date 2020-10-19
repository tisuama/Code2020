#ifndef HEAPTIMER__H
#define HEAPTIMER__H

#include "../cpplang.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef std::chrono::time_point<Clock> TimeStamp;//时间点

struct TimerNode{
    int id;//socket id
    TimeStamp expires;
    TimeoutCallBack cb;
    bool operator<(const TimerNode& t)
    {
        return expires<t.expires;
    }
};


class HeapTimer{
public:
    HeapTimer(){heap.reserve(64);}//调整
    ~HeapTimer(){clear();}
    void adjust(int id,int new_expires);
    void add(int id,int timeout,const TimeoutCallBack& cb);
    void dowork(int id);
    void clear();
    void tick();
    void pop();
    int GetNextTick();
    TimerNode top();

private:
    void del(int i);
    void shiftup(int i);
    bool shiftdown(int i,int n);
    void swapnode(int i,int j);
private:
    std::vector<TimerNode> heap;
    std::unordered_map<int,int> ref;
};


#endif