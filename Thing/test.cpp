#include<iostream>
using namespace std;

//测试局部静态变量的作用域
int* fun()
{
    static int x=1;
    return &x;
}

//测试是私有的静态成员
class A
{
public:
    static int x;//访问时出错，未定义，如果为private则不能访问
    const static int y;
};
int A::x=1;
int main()
{
    auto p=fun();
    cout<<*p<<endl;
    A a;
    cout<<A::x<<endl;
}