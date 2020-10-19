#include<iostream>
#include<atomic>
using namespace std;

//适用：持有时间短，不希望再调度上花费太多时间的情况
//CAS 比较并替换，多CPU环境下为硬件级别的原子操作(锁总线)
// bool CAS(V,A,B)
// {
//     if(V==A)
//     {
//         swap(V,B);
//         return true;
//     }
//     return false;
// }
//       ||
//       ||
//       \/
// b=true;
// while(!CAS(flag,false,b))//自旋
// //do something here
// flag=false;


//C++11原子量实现自旋锁
class SpinLock
{
public:
    SpinLock():flag(false){}
    void lock()
    {
        bool expect=false;
        while(!flag.compare_exchange_weak(expect,true))
            expext=false;
    }
    void unlock()
    {
        flag.store(false);//原子写操作
    }
private:
    std::atomic<bool> flag;
};

int main()
{

}