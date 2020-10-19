#include<iostream>
using namespace std;
class A
{
private:
    int m_a;
public:
    A(int a);
    A();
    A(A& a);
    void operator=(int x);
};
A::A()
{
    cout<<"construct"<<endl;
    m_a=999;
}
A::A(int a)
{
    cout<<"construct int a"<<endl;
    m_a=99;
}
A::A(A& a)
{
    cout<<"copy construct"<<endl;
    m_a=a.m_a;
}
void A::operator=(int x)
{
    m_a=x;
    cout<<"operator="<<endl;
}
int main()
{
    A a;
    a=5;
}