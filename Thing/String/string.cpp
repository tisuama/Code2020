#include<iostream>
using namespace std;
class String
{
public:
    String():data(new char[1])
    {
        *data='\0';
    }
    String(const char* str):data(new char[strlen(str)+1])
    {
        strcpy(data,str);//拷贝构造就完事了
    }
    String(const String& rhs):data(new char[rhs.size()+1])
    {
        strcpy(data,rhs.data);
    }
    String(String&& rhs):data(rhs.data)
    {
        rhs.data=nullptr;
    }
    ~String()
    {
        delete[] data;  
    }
    String& operator=(String rhs)//直接看使用拷贝构造还是移动构造
    {
        swap(rhs);//移动赋值得话直接交换就完事了
        return *this;
    }
    void swap(String& rhs)
    {
        using std::swap;
        swap(data,rhs.data);
    }
    size_t size()const
    {
        return strlen(data);
    }
    const char* c_str()
    {
        return data;
    }
private:
    char* data;
};


int main()
{

}