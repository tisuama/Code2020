#include<iostream>
#include<vector>
using namespace std;
// void f(int i)
// {
//     cout<<"void f(int i)"<<endl;
// }

void f(char* p)
{
    cout<<"void f(void* p)"<<endl;
}

// void f(nullptr_t)
// {
//     cout<<"nullptr_t"<<endl;
// }
int main()
{
    f(NULL);
    if(NULL==0) cout<<"NULL==0"<<endl;
    //f(nullptr); 
    f(0);
    nullptr_t n1;//nullptr 类型
    //f(n1);
}