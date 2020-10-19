#include<iostream>
using namespace std;
class B
{
public:
    void f(){g();}
    virtual void g(){cout<<"B::g"<<endl;}
};

class D:public B
{
public:
    void g(){cout<<"D:g"<<endl;}
};


int main()
{
    D d;
    d.f();
    B* b;
    b=&d;
    b->f();
}
