#include<iostream>
#include<vector>
#include<tuple>
using namespace std;

//case 1: 可变参数函数
template<typename T>
void _hash(size_t& seed,const T& val)
{
    cout<<"hash over "<<val<<endl;
}
template<typename T,typename ...Args>
void _hash(size_t& seed,const T& val,const Args& ...args)
{
    cout<<"parameter "<<val<<endl;
    _hash(seed,args...);
}
template<typename ...Args>
size_t _hash(const Args& ...args)
{
    cout<<"hash start"<<endl;
    cout<<"sizeof ... "<<sizeof...(args)<<endl;
    size_t seed=10;
    _hash(seed,args...);
    return seed;
}

//case 2: 可变参数类
template<typename ...Args>
class A
{
private:
    int size=0;//C++11支持
public:
    A()
    {
        size=sizeof...(Args);
        cout<<size<<endl;
    }
};

int main()
{
    size_t f=10;
    _hash("start",2,3,"end");
    _hash<string,int,int,string>("start",2,3,"end");//带参数调用

    A<int,string,vector<int>> a;
    

    //tuple
    tuple<int,string> t=make_tuple(1,"haha");
    int firstArg=get<0>(t);
    string secondArg=get<1>(t);//模板实现
    cout<<firstArg<<" "<<secondArg<<endl;
}