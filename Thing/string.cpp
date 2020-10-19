#include<iostream>
using namespace std;

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
    string(const char *str):data(new char[strlen(str)+1])
    {
        strcpy(data,str);
    }
    string(const string& str):data(new char[str.size()+1])
    {
        strcpy(data,str.data);
    }
    string(string&& str):data(str.data)
    {
        str.data=nullptr;
    }
    string& operator=(string rhs)
    {
        swap(rhs);
        return *this;
    }
    void swap(string& rhs)
    {
        using std::swap;
        swap(data,rhs.data);
    }
    int size()const 
    {
        return strlen(data);
    }
private:
    char* data;
};


int main()
{

}