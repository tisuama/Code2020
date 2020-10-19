#include<iostream>
using namespace std;

typedef void (*Fun)();


class Base
{
public:
    Base(){};
    virtual void fun1()
    {
        cout<<"Base:fun1()"<<endl;
    }
    virtual void fun2()
    {
        cout<<"Base:fun2()"<<endl;
    }
    virtual void fun3(){}
    ~Base(){}
};

class Derived:public Base
{
public:
    Derived(){};
    void fun1()
    {
        cout<<"Derived:fun1()"<<endl;
    }
    void fun2()
    {
        cout<<"Derived:fun2()"<<endl;
    }
    ~Derived(){}
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
    Base ptr;
    Derived d;
    Base *pt=new Derived();
    Base &pp=ptr;
    Base &p=d;
    cout<<"基类对象直接调用"<<endl;
    ptr.fun1();
    cout<<"基类对象调用基类实例"<<endl;
    pp.fun1();
    cout<<"基类指针指向派生类实例并调用虚函数"<<endl;
    pt->fun1();
    cout<<"基类引用指向派生类实例并调用虚函数"<<endl;
    p.fun1();

    //手动查找vptr和vtable
    Fun f1=getAddr(pt,0);
    (*f1)();
    Fun f2=getAddr(pt,1);
    (*f2)();
    delete pt;
}