#include<iostream>
using namespace std;

class X
{
public:
    int operator()(int i=0)
    {
        cout<<"X::operator( "<<i<<")"<<endl;return i;
    }
    int operator[](int i)
    {
        cout<<"X::operator[ "<<i<<"]"<<endl;return i;
    }
};

int main()
{
    X x;
    int i=x(2);
    int j=x[1];
    cout<<i<<" "<<j<<endl;
}