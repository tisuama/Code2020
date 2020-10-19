#include<iostream>
using namespace std;

class A
{
public:
    static int i;
};
int A::i=1;

int main()
{
    A a,b;
    a.i=2;
    b.i=3;
    cout<<a.i<<endl;
}