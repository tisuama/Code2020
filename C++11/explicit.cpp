#include<iostream>
using namespace std;
class Fraction
{
public:
    Fraction(int num,int den=1):a(num),b(den){}
    Fraction operator+(const Fraction& f)
    {
        return Fraction(this->a+f.a,b);
    }
    void get()
    {
        cout<<a<<" "<<b<<endl;
    }

private:
    int a,b;
};
class Fraction1
{
public:
    explicit Fraction1(int num,int den=1):a(num),b(den){}
    Fraction1 operator+(const Fraction1& f)
    {
        return Fraction1(this->a+f.a,b);
    }
    void get()
    {
        cout<<a<<" "<<b<<endl;
    }
private:
    int a,b;
};
class P
{
public:
    P(int a,int b)
    {
        cout<<"P(int a,int b"<<endl;
    }
    P(int a,int b,int c)
    {
        cout<<"non-explicit P"<<endl;
    }
    explicit P(int a,int b,int c,int d)
    {
        cout<<"explicit P"<<endl;
    }
};


int main()
{
    Fraction f(3,5);
    Fraction ff=f+3;
    ff.get();

    // Fraction1 f1(3,5);
    // Fraction1 ff1=f1+3;//explicit 
    // ff1.get();    

    P p1{77,5};
    P p2{77,5,89};
    //P p3={77,4,2,1};//注意和=和不带=的区别
}