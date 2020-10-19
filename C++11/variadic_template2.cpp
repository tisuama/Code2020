#include<iostream>
using namespace std;
void print(const char* s)
{
    while(*s)
    {
        if(*s=='%'&&*(++s)!='%')
            throw std::runtime_error("invalid format string");
        std::cout<<*s++;
    }
}
template<typename T,typename ...Args>
void print(const char* s,T val,Args ...args)
{
    while(*s)
    {
        if(*s=='%'&&*(++s)!='%')
        {
            std::cout<<val;
            print(++s,args...);
            return;
        }
        std::cout<<*s++;
    }
    throw std::logic_error("extra arguments");
}

int main()
{
    int* pi=new int;
    print("%d %s %p,%f\n",15,"Acc",pi,3.14);
}