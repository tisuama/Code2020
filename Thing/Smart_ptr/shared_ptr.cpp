#include<iostream>
using namespace std;
class shared_count
{
public:
    shared_count():count(1){}
    void add_count(){++count;}
    long reduce_count(){return --count;}
    long get_count()const{return count;}

private:
    long count;
};

template<class T>
class shared_ptr
{
public:
    shared_ptr(T* ptr=nullptr):ptr_(ptr)
    {
        if(ptr)
            shared_count_=new shared_count();
    }
    ~shared_ptr()
    {
        if(ptr_&&!shared_count_->reduce_count())
        {
            delete ptr_;
            delete shared_count_;
        }
    }

    operator bool(){return ptr_;}
    T* get() const{return ptr_;}

    //拷贝构造
    template<typename U>
    shared_ptr(shared_ptr<U>& rhs)//release
    {
        cout<<"copy construct"<<endl;
        ptr_=rhs.ptr_;
        if(ptr_)
        {
            rhs.shared_count_->add_count();
            shared_count_=rhs.shared_count_;
        }
    }
    //移动构造
    template<typename U>
    shared_ptr(shared_ptr<U>&& rhs)
    {
        cout<<"move construct"<<endl;
        ptr_=rhs.ptr_;
        if(ptr_)
        {
            shared_count_=rhs.shared_count_;
            rhs.shared_count_=nullptr;//并没有delete
            rhs.ptr_=nullptr;
        }
    }
    shared_ptr& operator=(shared_ptr rhs)//需要swap，简单写法
    {
        swap(rhs);
        return *this;
    }

    void swap(shared_ptr& rhs)
    {
        using std::swap;
        swap(ptr_,rhs.ptr_);
        swap(shared_count_,rhs.shared_count_);
    }

    long use_count()const
    {
        if(ptr_)
            return shared_count_->get_count();
        else
            return 0;
    }


    //转换
    template<typename U>
    shared_ptr(const shared_ptr<U>& rhs,T* ptr)
    {
        ptr_=ptr;
        if(ptr_)
        {
            rhs.shared_count_->add_count();
            shared_count_=rhs.shared_count_;
        }
    }

public:
    template<typename U>
    friend class shared_ptr;
private:
    T* ptr_;
    shared_count* shared_count_;
};

//   shared_ptr<B> dptr1=dynamic_pointer_cast<B>(dptr2);
template<class T,class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& other)
{
    T* ptr=dynamic_cast<T*>(other.get());//注意const对象不能调用非const函数
    //if(ptr==nullptr) cout<<"nullptr"<<endl;
    return shared_ptr<T>(other,ptr);
}

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
    //还会生成默认拷贝和默认移动构造函数
    shared_ptr<B> ptr1(new B());
    cout<<"use_count of ptr1 "<<ptr1.use_count()<<endl;
    shared_ptr<A> ptr2,ptr3;
    cout<<"use_count of ptr2 "<<ptr2.use_count()<<endl;
    ptr3=ptr2;//调用的时默认构造函数，计数器不会发生变化
    ptr2=ptr1;
    ptr3=ptr1;
    cout<<"use_count of ptr1 "<<ptr1.use_count()<<endl;
    cout<<"use_count of ptr2 "<<ptr2.use_count()<<endl;
    cout<<"use_count of ptr3 "<<ptr3.use_count()<<endl;
    if(ptr1)
        cout<<"ptr1 is not empty"<<endl;
    ptr2=std::move(ptr1);
    if(ptr2&&!ptr1)
    {
        cout<<"ptr1 move to ptr2"<<endl;
        cout<<"use_count of ptr1 "<<ptr1.use_count()<<endl;
        cout<<"use_count of ptr2 "<<ptr2.use_count()<<endl;
        cout<<"use_count of ptr3 "<<ptr3.use_count()<<endl;
    }


    A* a=new A();
    B* b;
    b=dynamic_cast<B*>(a);
    if(b!=nullptr)
        b->show();

    cout<<"==================="<<endl;

    shared_ptr<A> dptr2(new A);
    cout << "use count of dptr2 is now " << dptr2.use_count() << endl;    // 1
    shared_ptr<B> dptr1=dynamic_pointer_cast<B>(dptr2);
    cout << "use count of dptr1 is now " << dptr1.use_count() << endl;    // 0
    cout << "use count of dptr2 is now " << dptr2.use_count() << endl;    // 1

    cout<<"==================="<<endl;

    shared_ptr<A> dptr3(new B);
    shared_ptr<B> dptr4=dynamic_pointer_cast<B>(dptr3);
    cout << "use count of dptr1 is now " << dptr3.use_count() << endl;    // 0
    cout << "use count of dptr2 is now " << dptr4.use_count() << endl;    // 1        

    cout<<"==================="<<endl;

    shared_ptr<B> dptr5(new B);
    shared_ptr<A> dptr6=dynamic_pointer_cast<A>(dptr5);
    cout << "use count of dptr1 is now " << dptr5.use_count() << endl;    // 0
    cout << "use count of dptr2 is now " << dptr6.use_count() << endl;    // 1       

}