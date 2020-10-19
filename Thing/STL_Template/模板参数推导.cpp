#include <iostream>
using namespace std;
template<typename T>
T f(const T& param) // 此时T被推导为int, param的类型为const int &
{
    cout << param << endl;
    T tmp = 2019; // 正常运行
    tmp += 1;
    cout << tmp << endl;
    return tmp;
}
template<typename T>
void g(T& param) // 此时T被推导为const int, param的类型为const int &
{
    cout << param << endl;
    T tmp = 2019; // error，无法编译
    //tmp += 1;
    cout << tmp << endl;
}

//
int main()
{
    int x = 27; // as before
    const int& rx = x; // as before
    f(rx); // T is int
    g(rx); // T is const int
    return 0;
}