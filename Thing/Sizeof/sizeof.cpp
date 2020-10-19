#include<iostream>
using namespace std;
#pragma pack(4)

class A
{
    char a;
    char b;
    double c;
    // short b;
    //long long c;//win64 4字节
    // short e;
    //short f;
};


int main()
{
    cout<<sizeof(A)<<endl;
    long c;
    cout<<sizeof(c)<<endl;
}