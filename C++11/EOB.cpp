#include<iostream>
#include<cassert>
using namespace std;

class Empty
{
public:
    void print()
    {
        std::cout<<"I am empty class"<<endl;
    }
};
template<typename T>
bool isSame(T const& t1,T const& t2)
{
    return &t1==&t2;
}

//空基类优化
int main()
{
    cout<<sizeof(Empty)<<endl;
    Empty a,b;
    assert(!isSame(a,b));

    Empty *p=new Empty;
    Empty *q=new Empty;
    assert(!isSame(p,q));
}