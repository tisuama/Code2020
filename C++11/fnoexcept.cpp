#include<iostream>
#include<vector>
#include<string> 
using namespace std;
 
class Test
{
public:
    Test(const string& s = "hello world") :str(new string(s)) { cout << "默认构造函数" << endl; };
    Test(const Test& t);
    Test& operator=(const Test& t);
    Test(Test&& t)noexcept;
    Test& operator=(Test&& t)noexcept;
    ~Test();
public:
    string * str;
};
 
Test::Test(const Test& t)
{
    str = new string(*(t.str));
    cout << "拷贝构造函数" << endl;
}
Test& Test::operator=(const Test& t)
{
    cout << "拷贝赋值运算符" << endl;
    return *this;
}
Test::Test(Test&& t)noexcept
{
    str = t.str;
    t.str = nullptr;
    cout << "移动构造函数" << endl;
}
Test& Test::operator=(Test&& t)noexcept
{
    cout << "移动赋值运算符" << endl;
    return *this;
}
Test::~Test()
{
    cout << "析构函数" << endl;
}
 
int main()
{
    vector<Test> vec(1);
    Test t("what");
    vec.push_back(std::move(t));
    return 0;
}

/*
默认构造函数
默认构造函数
移动构造函数
移动构造函数
析构函数
析构函数
析构函数
析构函数
*/

/*
默认构造函数
默认构造函数
移动构造函数
拷贝构造函数
析构函数
析构函数
析构函数
析构函数
*/