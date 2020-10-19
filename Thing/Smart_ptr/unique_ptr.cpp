#include<iostream>
using namespace std;
template<class T>
class unique_ptr
{
public:
    unique_ptr(T* ptr=nullptr):ptr_(ptr){};
    ~unique_ptr()
    {
        delete ptr_;
    }
    T& operator*(){ return *ptr_; }
    T* operator->(){ return &(operator*()); }
    operator bool(){ return ptr_; }
    T* get() {return ptr_; }
    //没有拷贝构造了
    unique_ptr(unique_ptr&& other)//移动构造
    {
        ptr_=other.release();
    }
    template<class U>
    unique_ptr(unique_ptr<U>&& other)
    {
        ptr_=other.release();
    }
    T* release()
    {
        T* p=ptr_;
        ptr_=nullptr;
        return p;
    }
    unique_ptr& operator=(unique_ptr rhs)
    {
        unique_ptr tmp(rhs.release());
        swap(tmp);
        return *this;
    }
    void swap(unique_ptr& rhs)
    {
        using std::swap;
        swap(ptr_,rhs.ptr_);
    }

private:
    T*  ptr_;
};

class A
{
public:
    A(){}
    virtual void show(){cout<<"hehe"<<endl;}
    virtual ~A(){}
};

class B:public A
{
public:
    B(){}
    void show(){cout<<"haha"<<endl;}
    ~B(){}
};


int main()
{
    unique_ptr<A> ptr1(new B());
    unique_ptr<A> ptr2(std::move(ptr1));
    if(ptr2.get()!=nullptr&&ptr1.get()==nullptr)
        ptr2->show();
    unique_ptr<B> ptr3(new B());
    unique_ptr<A> ptr4(std::move(ptr3));//没有模板，这种移动构造不支持
    
    //unique_ptr<B> ptr5(std::move(ptr1));
}