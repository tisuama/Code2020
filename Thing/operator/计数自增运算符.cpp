#include<iostream>
using namespace std;
class Counter
{
    int r;
public:
    Counter(int i=0):r(i){}
    Counter& operator++();//前向
    Counter operator++(int);
    friend Counter& operator--(Counter& c);
    friend Counter operator--(Counter& c,int);

};

Counter& Counter::operator++()
{
    ++r;
    return *this;
}
Counter Counter::operator++(int)//不要返回局部对象的引用
{
    Counter c=*this;
    ++r;
    return c;
}
Counter& operator--(Counter& c)
{
    --c.r;
    return c;
}
Counter operator--(Counter& c,int)
{
    c.r--;
    return c;
}

int main()
{

}