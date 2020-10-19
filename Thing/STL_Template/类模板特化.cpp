#include<iostream>
using namespace std;
const int SIZE=5;
template<class T>
class Array
{
private:
    T A[SIZE];
public:
    Array()
    {
        for(int i=0;i<SIZE;i++)
            A[i]=0;
    }
    T& operator[](int);
    void Sort();
};
template<class T>
T& Array<T>::operator[](int i)
{
    if(i<0||i>=SIZE)
        exit(1);
    return A[i];
}

template<class T>
void Array<T>::Sort()
{
    for(int i=0;i<SIZE;i++)
    {
        for(int j=SIZE-1;j>i;j--)
        {
            if(A[j-1]>A[j])
                swap(A[j-1],A[j]);
        }
    }
}
template<>
void Array<char*>::Sort()
{
    cout<<"Array<char*>"<<endl;
    for(int i=0;i<SIZE;i++)
    {
        for(int j=SIZE-1;j>i;j--)
        {
            if(strcmp(A[j-1],A[j])>0)
                swap(A[j-1],A[j]);
        }
    }    
}

int main()
{
    Array<int> a1;
    Array<char*> b1;
    for(int i=4;i>=0;i--)
        a1[i]=i;
    a1.Sort();
    cout<<a1[0]<<endl;

    b1[0]="a",b1[1]="b";
    b1[2]="a",b1[3]="b";b1[4]="b";
    b1.Sort();
    cout<<b1[0]<<endl;
}
