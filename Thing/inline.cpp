#include<iostream>
using namespace std;
class Base
{
public:
    inline virtual void who(){cout<<"base who"<<endl;}
    virtual ~Base(){}
};

class Derived:public Base
{
public:
    inline void who(){cout<<"derived who"<<endl;}
};

int main()
{
    Base b;
    b.who();


    Base* ptr=new Derived();
    ptr->who();

    delete ptr;
    ptr=nullptr;
}