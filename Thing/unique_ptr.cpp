#include<iostream>
using namespace std;
template<class T>
class unique_ptr
{
public:
    unique_ptr(T* ptr_=nullptr):ptr(ptr_){}
    ~unique_ptr()
    {
        delete ptr;
    }
    unique_ptr(unique_ptr&& rhs)
    {
        ptr=rhs.release();
    }
    T* release()
    {
        T* p=ptr;
        ptr=nullptr;
        return p;
    }
    unique_ptr& operator=(unique_ptr rhs)
    {
        swap(rhs);
        return *this;
    }
    void swap(unique_ptr& rhs)
    {
        using std::swap;
        swap(ptr,rhs.ptr);
    }
private:
    T* ptr;
};


int main()
{

}