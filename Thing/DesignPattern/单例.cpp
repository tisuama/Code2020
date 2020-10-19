#include<iostream>
#include<thread>
#include<memory>
#include<mutex>
#include<atomic>
using namespace std;


//饿汉模式
class Singleton
{
public:
    Singleton* Instance()
    {
        return p;
    }
private:
    Singleton(){}
    static Singleton* p;
};
Singleton* Singleton::p=new Singleton();

//懒汉C++14
class Singleton1
{
public:
    Singleton1* Instance()
    {
        static Singleton1 p;//局部静态变量 可以访问
        return &p;
    }
private:
    Singleton1(){}
};


class Singleton2
{
public:
    Singleton2* Instance()
    {
        if(p==nullptr)
        {
            std::lock_guard<std::mutex> gurad(mtx);
            if(p==nullptr)
            {
                p=new Singleton2();
                return p;
            }
        }
    }
    class Gar
    {
        public:
            ~Gar()
            {
                if(Singleton2::p)
                    delete Singleton2::p;
            }
    };
    static Gar g;
private:
    Singleton2(){}
    static Singleton2* p;//针对指针的
    std::mutex mtx;
};
Singleton2* Singleton2::p=nullptr;
Singleton2::Gar Singleton2::g;


class Singleton3
{
public:
    Singleton3* Instance()
    {
        Singleton3* tmp=p.load(memory_order_relaxed);
        atomic_thread_fence(memory_order_acquire);
        if(tmp==nullptr)
        {
            std::lock_guard<std::mutex> guard(mtx);
            tmp=p.load(memory_order_relaxed);
            if(tmp==nullptr)
            {
                tmp=new Singleton3();//1 2 3一堆问题
                atomic_thread_fence(memory_order_release);
                p.store(tmp,memory_order_relaxed);
            }
        }
        return p;
    }

private:
    Singleton3(){}
    std::mutex mtx;
    std::atomic<Singleton3*> p;
};



int main()
{

}