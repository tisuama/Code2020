#include<iostream>
using namespace std;
class A
{
public:
    void f(int i){cout<<"A::f"<<endl;}
};
class B:public A
{
public:
    virtual void f(int i){cout<<"B::f"<<endl;}
};
class C:public B
{
public:
    void f(int i){cout<<"C::f"<<endl;}
    void show(int i){cout<<"C:show"<<endl;}
};
class D:public C
{
public:
    void f(int ){cout<<"D::f"<<endl;}
    void show(){cout<<"D::show"<<endl;}
};
int main()
{
    A *pa,a;
    B *pb,b;
    C c;
    D d;
    pa=&a;pa->f(1);
    pb=&b;pb->f(1);
    pb=&c;pb->f(1);
    pb=&d;pb->f(1);

    C* pc=&d;
    pc->show(1);
    d.C::show(1);//这样也能访问
    //d.show(1);不能访问，同名隐藏。基类可以访问
    c=d;
    c.show(1);
}