#include<iostream>
using namespace std;
class Figure
{
protected:
    double x,y;
public:
    void set(double i,double j)
    {
        x=i,y=j;
    }
    virtual void area()=0;
};
class Triangle:public Figure
{
public:
    void area()
    {
        cout<<"Triangle area "<<x*y*0.5<<endl;
    }
};
class Rectangle:public Figure
{
public:
    int r;
    void area()
    {
        cout<<"Retangle area "<<x*y<<endl;
    }
};


int main()
{
    //定义抽象类指针
    Figure *pf=NULL;
    Rectangle r;
    Triangle t;
    t.set(10,20);
    pf=&t;
    pf->area();
    //cout<<pf->r<<endl;
    r.set(10,20);
    pf=&r;
    pf->area();

    //定义抽象类引用 多态
    Figure& pF=t;
    pF.set(20,20);
    pF.area();
    //cout<<pF.r<<endl;
}