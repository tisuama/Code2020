#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;

int x=1,y=0;
int count=1;
std::mutex mtx;     
std::condition_variable cond1,cond2;
void print1()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(x>0&&count<=10)
        {
            cout<<"print1 "<<count<<endl;
            count++;
            y=1;
            x=0;
            lock.unlock();
            cond2.notify_one();
        }
        else if(count>10)
            return ;
        else 
            cond1.wait(lock);
    }
}
void print2()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(y&&count<=10)
        {
            cout<<"print2 "<<count<<endl;
            count++;
            x=1;
            y=0;
            lock.unlock();
            cond1.notify_one();
        }
        else if(count>10)
            return ;
        else
            cond2.wait(lock);
    }
}
int main()
{
    std::thread t1(print1);
    std::thread t2(print2);
    t1.join();
    t2.join();
}