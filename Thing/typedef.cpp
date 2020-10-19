#include<iostream>
using namespace std;

typedef int INT;
typedef void (*f)(int);//声明一种类型

void fun(int x)
{
    cout<<x<<endl;
}

int main()
{
    INT x=1;
    cout<<x<<endl;
    f f1=fun;
    f1(2);
}