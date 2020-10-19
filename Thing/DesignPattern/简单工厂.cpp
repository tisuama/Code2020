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

class Factory
{
public:
    AbstructProduct* getProduct(string name)
    {
        AbstructProduct* pro=NULL;
        if(name=="BasketBall")
            pro=new BasketBall();
        else
            pro=new FootBall();
        return pro;
    }
};

int main()
{
    Factory* f=new Factory();
    AbstructProduct* p=NULL;
    p=f->getProduct("BasketBall");
    p->play();//这里还是有问题
}