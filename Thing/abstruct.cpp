#include<iostream>
using namespace std;

class A
{
public:
    virtual void f()=0;
    void g(){this->f();}
    A(){};
};
class B:public A
{
public:
    void f(){cout<<"B::f"<<endl;}
};




/////////


// class Base
// {
// public:
//     virtual void show()=0;
//     int GetX(){return x;}
// private:
//     int x;
// };

// class Derived:public Base
// {
// public:
//     void show(){cout<<"In Derived"<<endl;}
//     Derived(){}
// };


///
class Base
{
public:
    Base(){cout<<"Construct"<<endl;}
    virtual ~Base(){cout<<"Destruct"<<endl;}
    virtual void show()=0;
};

class Derived:public Base
{
public:
    Derived(){cout<<"Construct:Derived"<<endl;}
    ~Derived(){cout<<"Destruct:Derived"<<endl;}
    void show(){cout<<"In Derived func"<<endl;}
};




int main()
{
    // B b;
    // b.g();

    Base *bp=new Derived();
    bp->show();
    delete bp;
}