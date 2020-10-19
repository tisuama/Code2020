#include<iostream>
using namespace std;



int main()
{
    const int x=1;
    //case 1：语法错误
    //int* p1=&x;

    //case 2：不能通过const指针修改，即使指向非const对象
    int y=1;
    const int *p=&y;
    //*p=2;
}