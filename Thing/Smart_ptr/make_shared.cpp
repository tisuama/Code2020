#include<iostream>
#include<memory>
using namespace std;

class A
{
public:
    int x;

};

int main()
{
    shared_ptr<A> ptr=make_shared<A>();
    cout<<ptr.use_count()<<endl;
    shared_ptr<A> ptr1=make_shared<A>();    
    cout<<ptr1.use_count()<<endl;
}
