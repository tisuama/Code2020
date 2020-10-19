#include<iostream>
using namespace std;

class A
{
public:
    virtual void show()=0;
};

int main()
{
    cout<<sizeof(A)<<endl;
}