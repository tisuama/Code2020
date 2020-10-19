#include<iostream>
using namespace std;

class Base
{
    int x;
public:
    Base(int a)
    {
        x=a;
        cout<<"Base Construct "<<x<<endl;
    }
};

class Derived:public Base
{
    int y;
public:
    Derived(int a,int b):Base(a)
    {
        y=b;
        cout<<"Derived Construct "<<y<<endl;
    }
    ~Derived(){cout<<"Derived Destruct"<<endl;}
};


int main()
{
    Derived(1,2);
}