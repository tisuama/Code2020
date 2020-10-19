#include<iostream>
using namespace std;
template<class T>
class auto_ptr
{
public:
    auto_ptr(T* ptr=nullptr):ptr_(ptr){}
    ~auto_ptr()
    {
        delete ptr_;
    }
    auto_ptr(auto_ptr& rhs)
    {
        ptr_=rhs.release();
    }
    auto_ptr& operator=(auto_ptr rhs)
    {
        swap(rhs);
        return *this;
    }
    T* release(auto_ptr& rhs)
    {
        T* ptr=rhs.ptr_;
        rhs.ptr_=nullptr;
        return ptr;
    }
    void swap(auto_ptr& rhs)
    {
        using std::swap;
        swap(ptr_,rhs.ptr_);
    }
private:
    T* ptr_;
};


int main()
{

}