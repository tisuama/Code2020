#include<iostream>
using namespace std;

typedef void (*Fun)();//定义函数指针

class A
{
public:
    virtual void fun()
    {
        cout<<"A::fun"<<endl;
    }
    virtual void fun1()
    {
        cout<<"A::fun1"<<endl;
    }
};
class B
{
public:
    virtual void fun()
    {
        cout<<"B::fun"<<endl;
    }
};

class MyC:public A,public B
{
public:
    virtual void fun()
    {
        cout<<"C::fun"<<endl;
    }    
    virtual void fun2()
    {
        cout<<"C::fun2"<<endl;
    }
};


Fun getAddr(void* obj,unsigned int offset)
{
    cout<<"============="<<endl;
    void* vptr_addr=(void*)*(unsigned long*)obj;
    cout<<"vptr_add:  "<<vptr_addr<<endl;

    void* func_addr=(void*)*((unsigned long*)vptr_addr+offset);//指针偏移一位
    cout<<"func_addr: "<<func_addr<<endl;
    return (Fun)func_addr;
}


int main()
{
    cout<<sizeof(MyC)<<endl;
    A* a=new MyC();
    a->fun();


    // Fun f1=getAddr(a,0);
    // (*f1)();
    // Fun f2=getAddr(a,1);
    // (*f2)();
    // Fun f3=getAddr(a,3);
    // (*f3)();段错误

    //a->fun2();//不可见的状态
    B* b=new MyC();
    b->fun();
    //b->fun1();

    A c=MyC();
    c.fun();//不好意思，不能实现多态
}

/*
class MyC       size(8):
        +---
 0      | +--- (base class A)
 0      | | {vfptr}
        | +---
 4      | +--- (base class B)
 4      | | {vfptr}
        | +---
        +---

MyC::$vftable@A@:
        | &MyC_meta
        |  0
 0      | &A::fun
 1      | &A::fun1
 2      | &MyC::fun2

MyC::$vftable@B@:
        | -4
 0      | &B::fun
 */