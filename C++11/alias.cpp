#include<iostream>
#include<vector>
#include<list>
using namespace std;

//case 1：alias用法
template<typename T>
using Vec=vector<T,allocator<T>>;


//case 2: 
template<typename T,template<typename U> class Container>
class XCLS
{
private:
    Container<T> c;
};

template<typename T>
using Lst=list<T>;


int main()
{
    //case 1
    Vec<int> col;


    //case 2
    XCLS<string,Lst> mylist;
}