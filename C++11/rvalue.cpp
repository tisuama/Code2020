#include<iostream>
#include<vector>
#include<complex>
using namespace std;

int foo(){return 5;}
class Foo
{
public:
    Foo()=default;
    Foo(const Foo& foo)=default;
    Foo(Foo&& foo)noexcept{}
};

void process(int& i)
{
    cout<<"左值传入"<<endl;
}
void process(int&& i)
{
    cout<<"右值传入"<<endl;
}
void UnPerfectForward(int&& i)
{
    cout<<"forward(int&& i)"<<endl;
    process(i);
}

//完美转发
void PerfectForward(int&& i)
{
    cout<<"forward(int&& i)"<<endl;
    process(static_cast<int&&>(i));
}
int main()
{
    int a=9,b=4;
    //a+b=42;
    
    string s1("hello");
    string s2("world");
    s1+s2="abc";
    string()="ok";
    cout<<s1<<" "<<s2<<" "<<s1+s2<<endl;

    //case 1
    int aa=1;
    process(aa);
    process(1);
    process(move(aa));

    UnPerfectForward(2);
    UnPerfectForward(move(aa));

    cout<<"--------------"<<endl;

    PerfectForward(2);
    PerfectForward(move(aa));
}