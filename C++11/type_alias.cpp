#include<iostream>
using namespace std;

//case 1
using fun=void (*)(int,int);

void example(int,int){}

//case 2
template<typename T>
struct Container
{
    using value_type=T;//等价于typedef T value_type=using value_type=T;
};

template<typename Cn>
void fun2(const Cn& cn)
{
    typename Cn::value_type n;//typename可以用在嵌套依赖中，表示类型
}

//case 3
template<typename T>
using string=basic_string<T,char_traits<T>>;

int main()
{
    fun fn=example;

}