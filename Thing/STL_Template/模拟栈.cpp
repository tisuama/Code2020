#include<iostream>
using namespace std;
//理解非类型参数和特化的区别
template<class T,int MAXSIZE>
class Stack
{
private:
    T elem[MAXSIZE];
    int top;
public:
    Stack(){top=0;}
    void push(T e);
    T pop();
    bool empty()
    {
        if(top<=-1)
            return 1;
        return 0;
    }
    void setEmpty()
    {
        top=-1;
    }
    bool full()
    {
        if(top>=MAXSIZE-1)
            return 1;
        return 0;
    }
};
template<class T,int MAXSIZE>
void Stack<T,MAXSIZE>::push(T e)
{
    if(full())
        return ;
    elem[top++]=e;
}
template<class T,int MAXSIZE>
T Stack<T,MAXSIZE>::pop()
{
    if(empty())
        return 0;
    return elem[--top];
}

int main()
{

}