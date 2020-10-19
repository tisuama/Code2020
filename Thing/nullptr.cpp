#include<iostream>
using namespace std;


void fun(int x)
{
    cout<<"fun1"<<endl;
}
// void fun(int* x)
// {
//     cout<<"fun2"<<endl;
// }

void show(int* x)
{
    cout<<(*x)<<endl;
}

int main()
{
    // fun(NULL);
    // fun(nullptr);
    void* p;
    int x=1;
    int* px=&x;
    p=px;
    cout<<*(int*)p<<endl;
    show((int*)p);//强转
    //fun(p);
}