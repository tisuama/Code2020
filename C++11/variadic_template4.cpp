#include<iostream>
#include<tuple>
#include<bitset>
using namespace std;


template<int IDX,int MAX,typename ...Args>
struct print_tuple
{
    static void print(ostream &os,const tuple<Args...> &t)
    {
        os<<get<IDX>(t)<<(IDX+1==MAX?"":",");
        print_tuple<IDX+1,MAX,Args...>::print(os,t);
    }
};

//偏特化
template<int MAX,typename... Args>
struct print_tuple<MAX,MAX,Args...>
{
    static void print(ostream& os,const tuple<Args...> &t){}
};

template<typename ...Args>
ostream& operator<<(ostream &os,const tuple<Args...> &t)
{
    os<<"[";
    print_tuple<0,sizeof...(Args),Args...>::print(os,t);
    return os<<"]";
}


int main()
{
    cout<<make_tuple(7.5,"hello",47)<<endl;
}