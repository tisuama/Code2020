#include<iostream>
using namespace std;

class Base
{
public:
    virtual void fun(int x=10)
    {
        cout<<"Base::fun"<<x<<endl;
    }
};
class Derived:public Base
{
public:
    virtual void fun(int x=20)
    {
        cout<<"Derived::fun"<<x<<endl;
    }
};
int main()
{
    Derived d1;
    Base* p=&d1;
    p->fun();
}