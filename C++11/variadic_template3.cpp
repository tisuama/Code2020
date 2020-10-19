#include<iostream>
using namespace std;

int max(initializer_list<int> List)
{
    int res=*List.begin();
    for(auto elem:List)
        res=max(res,elem);
    return res;
}

int maximum(int n)
{
    return n;
}
template<typename ...Args>
int maximum(int n,Args ...args)
{   
    return std::max(n,maximum(args...));
}

int main()
{
    cout<<maximum(10,8,100,11)<<endl;
}