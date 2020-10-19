#ifndef BLOCKQUEUE__H
#define BLOCKQUEUE__H


#include "../cpplang.h"


template<class T>
class BlockDeque{
public:
    explicit BlockDeque(int MaxCapacity=100);
    ~BlockDeque();
    void clear();
    bool empty();
    bool full();
    void Close();
    int size();
    int GetCapacity();
    T front();
    T back();
    void push_back(const T& item);
    void push_front(const T& item);
    bool pop(T& item);
    bool pop(T& item,int timeout);
    void flush();


private:
    std::deque<T> deq;
    int capacity;
    std::mutex mtx;
    bool isClose;
    std::condition_variable condConsumer;
    std::condition_variable condProducer;
};

//去掉static explcit
template<typename T>
BlockDeque<T>::BlockDeque(int MaxCapacity):capacity(MaxCapacity)
{
    assert(MaxCapacity>0);
    isClose=false;
}
template<typename T>
BlockDeque<T>::~BlockDeque()
{
    Close();
}
template<typename T>
void BlockDeque<T>::clear()
{
    std::lock_guard<std::mutex> guard(mtx);
    deq.clear();
}
template<typename T>
bool BlockDeque<T>::empty()
{
    std::lock_guard<std::mutex> guard(mtx);
    return deq.empty();    
}
template<typename T>
bool BlockDeque<T>::full()
{
    std::lock_guard<std::mutex> guard(mtx);
    return deq.size()>=capacity;
}
template<typename T>
void BlockDeque<T>::Close()
{
    {
        std::lock_guard<std::mutex> locker(mtx);
        deq.clear();
        isClose=true;
    }
    condProducer.notify_all();
    condConsumer.notify_all();
}
template<typename T>
int BlockDeque<T>::size()
{
    std::lock_guard<std::mutex> guard(mtx);
    return deq.size();    
}
template<typename T>
int BlockDeque<T>::GetCapacity()
{
    std::lock_guard<std::mutex> guard(mtx);
    return capacity;
}
template<typename T>
T BlockDeque<T>::front()
{
    std::lock_guard<std::mutex> guard(mtx);
    return deq.front();    
}
template<typename T>
T BlockDeque<T>::back()
{
    std::lock_guard<std::mutex> guard(mtx);
    return deq.back();    
}
template<typename T>
void BlockDeque<T>::push_back(const T& item)
{
    std::unique_lock<std::mutex> locker(mtx);
    //condProducer.wait(locker,[deq_size=deq.size(),capacity_=capacity]{return deq_size<capacity_;});
    while(deq.size()>=capacity)
        condProducer.wait(locker);
    deq.push_back(item);
    condConsumer.notify_one();
}
template<typename T>
void BlockDeque<T>::push_front(const T& item)
{
    std::unique_lock<std::mutex> locker(mtx);
    //condProducer.wait(locker,[deq_size=deq.size(),capacity_=capacity]{return deq_size<capacity_;});
    while(deq.size()>=capacity)
        condProducer.wait(locker);
    deq.push_front(item);
    condConsumer.notify_one();    
}
template<typename T>
bool BlockDeque<T>::pop(T& item)
{
    std::unique_lock<std::mutex> locker(mtx);
    // while(true)
    // {
    //     if(!deq.empty())
    //     {
    //         item=deq.front();
    //         deq.pop_front();
    //         condProducer.notify_one();
    //         locker.unlock();//可有可无
    //         return true;
    //     }
    //     else if(isClose) return false;
    //     else
    //         condConsumer.wait(locker);
    // }
    // return false;
    while(deq.empty())
    {
        condConsumer.wait(locker);
        if(isClose)
            return false;
    }
    item=deq.front();
    deq.pop_front();
    condProducer.notify_one();
    return true;
}
template<typename T>
bool BlockDeque<T>::pop(T& item,int timeout)
{
    std::unique_lock<std::mutex> locker(mtx);
    // while(deq.empty())
    // {
    //     if(isClose)
    //         return false;
    //     if(condConsumer.wait_for(locker,std::chrono::seconds(std::chrono::seconds(timeout)))==std::cv_status::timeout)//真唤醒
    //         return false;
    // }
    while(deq.empty())
    {
        if(condConsumer.wait_for(locker,std::chrono::seconds(timeout))==
        std::cv_status::timeout)
            return false;
        if(isClose)
            return false;
    }
    item=deq.front();
    deq.pop_front();
    condProducer.notify_one();
    return true;    
}
template<typename T>
void BlockDeque<T>::flush()
{
    condConsumer.notify_one();
}

#endif