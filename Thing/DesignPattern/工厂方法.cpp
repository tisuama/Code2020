#include<iostream>
using namespace std;

class AbstructProduct
{  
public:
    AbstructProduct(){}
    virtual void printName()=0;
    virtual void play()=0;
};
class BasketBall:public AbstructProduct
{
public:
    BasketBall(){printName();play();}
    void printName()
    {
        cout<<"Get BasketBall"<<endl;
    }
    void play()
    {
        cout<<"Play BasketBall"<<endl;
    }
};
class FootBall:public AbstructProduct
{
public:
    FootBall(){printName();play();}
    void printName()
    {
        cout<<"Get FootBall"<<endl;
    }
    void play()
    {
        cout<<"Play FootBall"<<endl;
    }
};



//--------------


class AbstructFactory
{
public:
    virtual AbstructProduct* getProduct()=0;
};

class BasketBallFactory:public AbstructFactory
{
public:
    BasketBallFactory()
    {
        cout<<"BasketBallFactory"<<endl;
    }
    AbstructProduct* getProduct()
    {
        return new BasketBall();
    }
};

class FootBallFactory:public AbstructFactory
{
public:
    FootBallFactory()
    {
        cout<<"FootBallFactory"<<endl;
    }
    AbstructProduct* getProduct()
    {
        return new FootBall();
    }
};

int main()
{
    AbstructFactory* f=NULL;
    AbstructProduct* p=NULL;
    f=new BasketBallFactory();
    p=f->getProduct();
}