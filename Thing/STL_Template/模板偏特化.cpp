#include<iostream>
using namespace std;
template<typename T1,typename T2>
class Test1
{
public:
    Test1(T1 i,T2 j):a(i),b(j){}
private:
    T1 a;
    T2 b;

};
template<>
class Test2<int,char>
{
public:
    Test2(int i,char j):a(i),b(j){}
private:
    int a;
    char b;
};

template<class T>
class Test3<char,T>
{
public:
    Test3(char i,T j):a(i),b(j){}
private:
    char i;
    T j;
};

int main()
{   
    Test1<double,double> t1(0,1);
    Test2<int,char> t2(2,'a');
    Test3<char,int> t3('a',2);
}