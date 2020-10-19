#include<iostream>
using namespace std;

class A
{
public:
    A(){cout<<"Construct A"<<endl;}
    ~A(){cout<<"Destruct A"<<endl;}
};

class B:public A
{
public:
    ~B(){cout<<"Destruct B"<<endl;}
};

int main()
{
    B b;
}