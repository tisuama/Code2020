#include<iostream>
using namespace std;
template<class T>
class auto_ptr
{
public:
    //一样效果
    explicit auto_ptr(T* ptr=nullptr):ptr_(ptr){}
    ~auto_ptr()
    {
        delete ptr_;
    }
    T& operator*(){ return *ptr_;  }
    T* operator->(){ return &(operator*()); }
    operator bool(){ return ptr_; }
    T* get()const{return ptr_;}

    //拷贝构造 auto_ptr本类型
    auto_ptr(auto_ptr& other)
    {
        ptr_=other.release();//=nullptr
    }
    T* release()
    {
        T* ptr=ptr_;//指针
        ptr_=nullptr;
        return ptr;
    }

    //copy and swap用法
    auto_ptr& operator=(auto_ptr &rhs)
    {
        auto_ptr tmp(rhs.release());
        swap(tmp);
        return *this;
    }
    //替代写法
    // auto_ptr& operator=(auto_ptr rhs)//调用拷贝构造或者移动构造
    // {
    //     swap(rhs);
    //     return *this;
    // }

    void swap(auto_ptr &rhs)//noexcept
    {
        using std::swap;
        swap(ptr_,rhs.ptr_);
    }

private:
    T* ptr_;
};

// template<class T>
// void swap(auto_ptr<T>& lhs,auto_ptr<T>&rhs)
// {
//     lhs.swap(rhs);
// }

class A
{
public:
    A(){}
    void show(){cout<<"hehe"<<endl;}
};
int main()
{
    auto_ptr<A> ptr1(new A());
    auto_ptr<A> ptr2(ptr1);
    if(ptr1.get()==nullptr&&ptr2.get())
        cout<<"拷贝构造，ptr1释放所有权,ptr2获得所有权"<<endl;
    ptr1=ptr1;
    auto_ptr<A> ptr3(new A());
    ptr1=ptr3;
    if(ptr3.get()==nullptr&&ptr1.get())
        cout<<"赋值运算符,始终只有一个ptr有控制权"<<endl;
}