#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<condition_variable>
using namespace std;
const int MAX_NUM=10000;

class BoundedBuffer
{
public:
    BoundedBuffer(size_t n)
    {
        array.resize(n);
        start=end=pos=0;
    }
    void produce(int n)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            full.wait(lock,[=]{return pos<array.size();});
            array[end]=n;
            end=(end+1)%array.size();
            ++pos;
            cout<<"Produce pos:"<<pos<<endl;
        }
        empty.notify_one();
    }
    int Consume()
    {
        std::unique_lock<std::mutex> lock(mtx);
        empty.wait(lock,[=]{return pos>0;});
        int n=array[start];
        start=(start+1)%array.size();
        --pos;
        cout<<"Consume pos:"<<pos<<endl;
        lock.unlock();
        full.notify_one();
        return n;
    }
private:
    std::vector<int> array;//循环数组实现的
    size_t start;
    size_t end;
    size_t pos;
    std::mutex mtx;
    std::condition_variable full;
    std::condition_variable empty;
};


int main()
{

}