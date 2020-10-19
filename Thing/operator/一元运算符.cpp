#include<iostream>
using namespace std;
class Time
{
int h,m,s;
public:
    Time(int hh,int mm,int ss):h(hh),m(mm),s(ss){}
    Time operator++();
    friend Time operator--(Time& t);
};

Time Time::operator++()
{
    ++s;
    if(s==60)
    {
        s=0;
        ++m;
        if(m==60)
        {
            ++h;
            if(h==24)
                h=0;
        }
    }
    return *this;
}

Time operator--(Time& t)//前向
{
    --t.s;
    if(t.s==0)
    {
        t.s=60;
        --t.m;
        if(t.m==0)
        {
            --t.h;
            if(t.h==0)
                t.h=24;
        }
    }
    return t;
}


int main()
{
    Time t(2,8,60);
    --t;

}
