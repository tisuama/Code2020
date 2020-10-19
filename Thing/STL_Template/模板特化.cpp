#include<iostream>
using namespace std;
template<class T>
T MAX(T a,T b)
{
    return a>b?a:b;
}
//特化
template<>
char* MAX<char*>(char *a,char *b)
{
    return strcmp(a,b)>=0?a:b;
}

int main()
{
    float c=5.1,d=3.2;
    cout<<MAX(c,d)<<endl;
    cout<<MAX("abc","acc")<<endl;
}