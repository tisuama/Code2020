#include<iostream>
using namespace std;
class X
{
public:
    X& operator=(const X& x)//a=b=c这种形式必须返回引用
    {
        cout<<"a:";return *this;
    }
};


int main()
{
    X x1,x2,x3;
    x1=x2;
    x1.operator=(x2);
    x1=x2=x3;

}