#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<condition_variable>
using namespace std;

int x=1,y=0;
int count=1;
mutex mtx;
condition_variable cond1,cond2;

void print1()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(count>10)
            return ;
        else if(x==1)
        {
            cout<<"thread1 "<<count<<endl;
            count++;
            x=0;
            y=1;
            cond2.notify_one();
        }
        else
            cond1.wait(lock);
    }
}

void print2()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(count>10)
            return ;
        else if(y==1)
        {
            cout<<"thread2 "<<count<<endl;
            count++;
            x=1;
            y=0;
            cond1.notify_one();
        }
        else
            cond2.wait(lock);
    }
}

const int MAXNUM=1000;
class Buffer
{
public:
    Buffer(int n)
    {
        A.resize(n);
        start=end=num=0;
    }
    void produce(int n)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond1.wait(lock,[=]{return num<A.size();});
        A[end]=n;
        end=(end+1)%A.size();
        cond2.notify_one();
        ++num;
    }
    void consumer()
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond2.wait(lock,[=]{return num>0;});
        int x=A[start];
        start=(start+1)%A.size();
        cond1.notify_one();
        --num;
    }
private:
    vector<int> A;
    mutex mtx;
    int start,end;
    int num;
    condition_variable cond1,cond2;
};

class string
{
public:
    string():data(new char[1])
    {
        *data='\0';
    }
    ~string()
    {
        delete[] data;
    }
    string(const string& str):data(new char[str.size()+1])
    {
        strcpy(data,str.data);
    }
    string(const char* str):data(new char[strlen(str)+1])
    {
        strcpy(data,str);
    }
    string(string&& str):data(str.data)
    {
        str.data=nullptr;//release
    }
    string& operator=(string rhs)
    {
        swap(rhs);
        return *this;
    }
    void swap(string& rhs)
    {
        using std::swap;
        swap(data,str.data);
    }
    int size()const
    {
        return strlen(data);
    }
private:
    char* data;
};


//smart ptr
template<typename T>
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
privaete:
    T* ptr;
};

template<typename T>
class auto_ptr
{
public:
    auto_ptr(T* ptr_=nullptr):ptr(ptr_){}
    ~auto_ptr()
    {
        delete ptr;
    }
    auto_ptr(auto_ptr& rhs)
    {
        ptr=rhs.release();
    }
    auto_ptr& operator=(auto_ptr rhs)
    {
        swap(rhs);
        return *this;
    }
    void swap(auto_ptr& rhs)
    {
        using std::swap;
        swap(ptr,rhs.ptr);
    }
    T* release(auto_ptr& rhs)
    {
        T* p=rhs.ptr;
        rhs.ptr=nullptr;
        return p;
    }
private:
    T* ptr;
};
// template<typename T>
// class shared_count
// {
// public:
//     shared_count():count(0){}
//     void add_count(){count++;}
//     void reduce_count(){count--;}
//     int get(){return count;}
// private:
//     int count;
// };
// class shared_ptr
// {
// private:
//     shared_count* count;
//     T* ptr;
// };

// char* strcpy(char* dest,char* src)
// {
//     if(!dest||!src) return NULL;
//     char* d=dest;
//     while((*dest++=*src++)!='\0');
//     return d;
// }
// void* memcpy(void* dest,void* src,size_t count)
// {
//     if(!dest||!src) return NULL;
//     char* d=(char*)dest;
//     char* s=(char*)src;
//     while(count--)
//         *d++=*s++;
//     return dest;
// }

int main()
{
    std::thread t1(print1);
    std::thread t2(print2);
    t1.join();
    t2.join();
}