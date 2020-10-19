#include "heaptimer.h"


void HeapTimer::add(int id,int timeout,const TimeoutCallBack& cb)
{
    //id唯一
    int i;
    if(ref.count(id)==0)
    {
        i=heap.size();
        ref[id]=i;
        heap.push_back({id,Clock::now()+MS(timeout),cb});
        shiftup(i);
    }
    else
    {
        i=ref[id];
        heap[i].expires=Clock::now()+MS(timeout);
        heap[i].cb=cb;
        if(!shiftdown(i,heap.size()))
            shiftup(i);
    }
}
void HeapTimer::adjust(int id,int new_expires)
{
    assert(ref.count(id)!=0);
    int i=ref[id];
    heap[i].expires=Clock::now()+MS(new_expires);
    shiftdown(i,heap.size());
}
void HeapTimer::dowork(int id)//删除节点,触发定时函数
{
    if(heap.empty()||ref.count(id)==0)
        return ;
    int i=ref[id];
    TimerNode node=heap[i];
    node.cb();
    del(i);
}
void HeapTimer::clear()
{
    ref.clear();
    heap.clear();
}
void HeapTimer::tick()//清除超时节点
{
    if(heap.empty())
        return;
    while(!heap.empty())
    {
        TimerNode node=heap.front();
        if(std::chrono::duration_cast<MS>(node.expires-Clock::now()).count()>0)
            break;
        node.cb();
        pop();
    }
}
void HeapTimer::pop()
{
    if(heap.empty())
        return ;
    del(0);
}
TimerNode HeapTimer::top()
{
    return heap[0];
}
int HeapTimer::GetNextTick()
{
    tick();
    size_t res=-1;
    if(!heap.empty())
    {
        res=std::chrono::duration_cast<MS>(heap.front().expires-Clock::now()).count();
        if(res<0)
            res=0;
    }
    return res;
}
void HeapTimer::del(int i)
{
    assert(!heap.empty()&&i>=0&&i<heap.size());
    int n=heap.size()-1;
    if(i>n) return ;
    if(i!=n)
    {
        swapnode(i,n);
        if(!shiftdown(i,n))
            shiftup(i);
    }
    ref.erase(heap.back().id);
    heap.pop_back();
}
void HeapTimer::shiftup(int i)//ref 
{
    assert(i>=0&&i<heap.size());
    if(!i) return;
    int j=(i-1)/2;
    while(j>=0)//parent
    {
        if(heap[j]<heap[i]) break;//parent
        swapnode(i,j);
        i=j;
        j=(i-1)/2;
    }
}
bool HeapTimer::shiftdown(int index,int n)
{
    assert(n>=0&&n<=heap.size());
    assert(index>=0&&index<heap.size());
    int i=index;
    int j=i*2+1;
    while(j<n)//child
    {
        if(j+1<n&&heap[j+1]<heap[j]) j++;
        if(heap[i]<heap[j]) break;
        swapnode(i,j);
        i=j;
        j=i*2+1;
    }
    return i>index;
}
void HeapTimer::swapnode(int i,int j)
{
    assert(i>=0&&i<heap.size());
    assert(j>=0&&j<heap.size());
    std::swap(heap[i],heap[j]);
    ref[heap[i].id]=i;
    ref[heap[j].id]=j;
}
