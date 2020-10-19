#include<iostream>
using namespace std;

enum Shpe{Circle,Retangle};
//auto Circle=10;

//case 1：namespace 定义
namespace Type
{
    enum Enum
    {
        Ground=0,
        Human,
        Aerial,
        Total
    };
}


//case 2：枚举类
enum class Type1
{
    Ground=0,
    Human,
    Aerial,
    Total
};


void foo(Type1 type)
{
    if(type==Type1::Ground)
        cout<<"Ground"<<endl;
}
int main()
{
    foo(Type1::Ground);
}