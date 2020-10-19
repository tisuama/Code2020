#include<iostream>
using namespace std;
class A
{
public:
    A(int x):a(x){}
    operator bool()//隐式转换
    {
        cout<<"operator bool"<<endl;
        return a;
    }
private:
    int a;
};




int main()
{
    A a(10);
    A b(10);
    if(a)//operator bool 类型
        cout<<"a"<<endl;
    if(a==b)
        cout<<"a==b"<<endl;
}