#include<iostream>
#include<functional>
using namespace std;

class A
{
public:
    A(){}
    // int operator()(){return 1;}
    void show(){cout<<"show"<<endl;}

public:
    int a=1;
    const int b=1;
    static int c;
    const static int d=1;
};
int A::c=1;

std::function<int(int,int)> f=[](auto x,auto y){return x+y;};

int main()
{
    //还是会产生错误
    // A a();
    A a=A();//哈哈哈，这样就对了
    a.show();

    //a b c d
    cout<<a.a<<" "<<a.b<<" "<<a.c<<" "<<a.d<<endl;

    //funciton
    // auto f=[](auto x,auto y){return x+y;};
    auto x=f(1,2);
    cout<<x<<endl;
}