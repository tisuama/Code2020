#include<iostream>
using namespace std;
class A
{
public:
    A():s(new char[12]){}
    ~A(){delete[] s;}
public:
    virtual void show(){cout<<"hehe parent"<<endl;}
    void haha(){show();}
    void hehe(){}
    int a,b;
    char* s;
};
class B:public A
{
public:
    
};

int main()
{
    cout<<sizeof(A)<<endl;
    A* a=new A();
    cout<<sizeof(*a)<<endl;
}