#include<iostream>
using namespace std;

//case 1: 仿函数
class CustomerHash
{
public:
    size_t operator()(const Customer& c)const 
    {
        // return ...
    }
};

//case 2：函数指针的方式
size_t customer_hash_func(const Customer& t)
{
    // return ...
}

//case 3: 偏特化的方式
template<>
struct hash<Customer>
{
    size_t operator()(const Customer& s)const noexcept
    {
        // return ....
    }
}

//case 4: 可变参数模板
template<typename T>
inline void hash_combine(size_t& seed,cosnt T& val)
{
     seed=std::hash<T>()(val)+0x9e3779b9;
            +(seed<<6)+(seed>>2);
}
template<typename T>
inline void hash_val(size_t& seed,const T&val)
{
    hash_combine(seed,val);
}
template<typename T,typename ...Types>
inline void hash_val(size_t& seed,const T& val,const Types& ...args)
{
    hash_combine(seed,val);
    hash_val(seed,args);
}
template<typename ...Types>
inline size_t hash_val(const Types& ...args)
{
    size_t seed=0;
    hash_val(seed,args...);
    return seed;
}


int main()
{

}