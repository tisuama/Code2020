#include<iostream>
using namespace std;


template<class T>
class scoped_ptr
{
public:
    explicit scoped_ptr(T* ptr=nullptr):ptr_(ptr){}
    ~scoped_ptr()
    {
        delete ptr_;
    }
    void reset(T* p=0)noexcept
    {
        scoped_ptr(p).swap(*this);
    }

    T& operator*(){ return *ptr_; }
    T* operator->(){ return &(operator*()); }
    T* get() const{ return ptr_; }

    void swap(scoped_ptr& rhs)
    {
        using std::swap;
        swap(ptr_,rhs.ptr_);
    }
 
private:
    T* ptr_;
    scoped_ptr(scoped_ptr &);
    scoped_ptr& operator=(scoped_ptr &);//只声明，不实现
};


class A
{
public:
    A(){}
    void show(){cout<<"hehe"<<endl;}
};


int main()
{
    scoped_ptr<A> ptr1(new A());
    // scoped_ptr pt2=ptr1;
    ptr1->show();

}