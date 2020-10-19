#include<iostream>
using namespace std;
class A
{
    int x;
public:
    A(int aa)
    {
        x=aa;
        cout<<"Construct A"<<endl;
    }
};
class B:public A
{
public:
    B(int x):A(x){cout<<"Construct B"<<endl;}
};
class C:public B
{
public:
    C(int y):B(y){cout<<"Construct C"<<endl;}
};

int main()
{
    C c(1);

}