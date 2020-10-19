#include<iostream>
using namespace std;


//final
//case 1
class Base final
{
public:
    Base(){}
    virtual void func(){}
};
class Derivered:public Base
{
};

//case 2
class Base1
{
public:
    Base1(){}
    virtual void func()final{}//表示不能再被重写了
};

int main()
{

}