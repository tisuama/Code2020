#include<iostream>
using namespace std;

extern const int ext;

class A
{
public:
    const int x=1;
    static const int y;
    int func(int x) const
    {
        x+=10;
        return x;
    }
};
const int A::y=2;


void fun1(const int x)
{
    //x=1;
    cout<<x<<endl;
}

int main()
{
    cout<<ext<<endl;
    A a;
    cout<<a.x<<endl;
   // a.x=2;//read only
    cout<<a.x<<endl;
    
    //fun1(2);

    cout<<a.func(2)<<endl;
}