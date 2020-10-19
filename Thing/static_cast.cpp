#include<iostream>
using namespace std;
class B
{
public:
    virtual void foo(){};
};

class D:public B
{
public:s
    void foo(){}
};

void func(B *pb)
{
    D* pd1=static_cast<D*>(pb);
    D* pd2=dynamic_cast<D*>(pb);
    if(pd2==NULL)
        cout<<"dynamic_cast NULL"<<endl;
}

int main()
{
    B* b=new B;
    func(b);
}