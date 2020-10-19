#include<iostream>
using namespace std;

template<class T>
void fun(T* A)
{
    T C[10];
    for(int i=0;i<10;i++)
        C[i]=*(A+i);
    for(int i=0;i<10;i++)
        cout<<C[i]<<endl;
}

class T
{
public:
    int a;
};

int main()
{
    int A[10];
    for(int i=0;i<10;i++)
        A[i]=i;
    fun(A);
}