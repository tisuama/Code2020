#include<iostream>
using namespace std;
class A
{
public:
    virtual ~A(){cout<<"call A::~A()"<<endl;}
};
class B:public A
{
    char *buff;
public:
    B(int i){buff=new char[i];}
    ~B()
    {
        delete [] buff;
        cout<<"call B::~B()"<<endl;
    }
};

int main()
{
    A* a=new B(10);
    delete a;//不显式的写的话 没有显示调用过程
}
