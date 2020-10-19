#include<iostream>
using namespace std;
class shared_count
{
public:
    shared_count():count(0){}
    void add_count(){++count;}
    long reduce_count(){return --count;}
    long get_count(){return count;}
private:
    long count;
};
template<class T>
class shared_ptr
{
public:
    shared_ptr(T* ptr_=nullptr):ptr(ptr_)
    {
        if(ptr)
            count=new shared_count();
    }
    ~shared_ptr()
    {
        if(ptr&&!count->reduce_count())
        {   
            delete ptr;
            delete count;
        }
    }
    shared_ptr(const shared_ptr& rhs)
    {
        ptr=rhs.ptr;
        if(ptr)
        {
            rhs.count->add_count();//add count太牛了
            count=rhs.count;
        }
    }
    shared_ptr(shared_ptr&& rhs)
    {
        ptr=rhs.ptr;
        if(ptr)
        {
            count=rhs.count;
            rhs.ptr=nullptr;
            rhs.count=nullptr;
        }
    }
    shared_ptr& operator=(shared_ptr rhs)//加不加1这里已经做完了。。。太强了吧
    {
        swap(rhs);
        return *this;
    }
    void swap(shared_ptr& rhs)
    {
        using std::swap;
        swap(ptr,rhs.ptr);
        swap(count,rhs.count);
    }
private:
    shared_count* count;
    T* ptr;
};


int main()
{

}