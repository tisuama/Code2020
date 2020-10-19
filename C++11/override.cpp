#include<iostream>
using namespace std;

class Base
{
public:
    Base(){}
    virtual void func(){};
};
class Derivered:public Base
{
    virtual void func(int) /*override*/{}
};
int main()
{

}