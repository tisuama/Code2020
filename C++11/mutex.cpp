#include<iostream>
#include<mutex>
#include<thread>
using namespace std;

int sum=0;
mutex m;
void count()
{
    int i;
    for(int i=0;i<10000;i++)
    {
        m.lock();
        sum+=1;
        m.unlock();
    }
}

int main()
{
    thread t1(count);
    thread t2(count);
    t1.join();
    t2.join();
    cout<<sum<<" "<<sum<<endl;
    return 0;
}