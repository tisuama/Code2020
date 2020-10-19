#include<iostream>
using namespace std;

//Summary：可以看出函数调用的规则

void printX()
{
    cout<<"printX-------1"<<endl;
}

template<typename T,typename ...Type>
void printX(const T& firstArg,const Type& ...args)
{
    cout<<"printX-------2"<<endl;
    cout<<firstArg<<endl;
    printX(args...);
}

template<typename T,typename ...Type>
void printX(const Type& ...args)
{
    cout<<"printX-------3"<<endl;
    printX(args...);
}
int main()
{
    printX(1,"hello",42);
}