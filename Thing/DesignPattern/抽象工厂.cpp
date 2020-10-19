#include<iostream>
using namespace std;

/*
简单工厂模式所有逻辑都封装在工厂类中，工厂根据客户提供的产品名字创建对应产品的对象实例；
工厂方法模式将产品的创建过程放到了具体工厂类中，每一个工厂可以创建一个具体产品，由此可能会创建许多工厂类。
很多时候，一个工厂不只是生产一种产品，而是生产一类产品，比如一个体育用品工厂，可以生产篮球、足球、排球
等多种产品。此时我们可以把这些相关的产品归纳为一个“产品族”，由同一个工厂来生产，这即是抽象工厂模式
*/

//------ ball
class AbstructBall
{
public:
    AbstructBall(){}
    virtual void play()=0;
};
class Basketball:public AbstructBall
{
public:
    Basketball(){}
    void play()
    {
        cout<<"play basketball"<<endl;
    }
};
class Football:public AbstructBall
{
public:
    Football(){}
    void play()
    {
        cout<<"play football"<<endl;
    }
};

//---------shirt
class AbstructShirt
{
public:
    AbstructShirt(){}
    virtual void wearshirt()=0;
};
class Basketballshirt:public AbstructShirt
{
public:
    Basketballshirt(){}
    void wearshirt()
    {
        cout<<"wear basketball shirt"<<endl;
    }
};
class Footballshirt:public AbstructShirt
{
public:
    Footballshirt(){};
    void wearshirt()
    {
        cout<<"wear football shirt"<<endl;
    }
};

class AbstructFactory
{
public:
    virtual AbstructBall* getBall()=0;
    virtual AbstructShirt* getshirt()=0;
};
class BasketballFactory:public AbstructFactory
{
public:
    BasketballFactory(){};
    AbstructBall* getBall()
    {
        cout<<"get basketball"<<endl;
        return new Basketball();
    }
    AbstructShirt* getshirt()
    {
        cout<<"get basketball shirt"<<endl;
        return new Basketballshirt();
    }
};


int main()
{
    AbstructFactory* f=new BasketballFactory();
    AbstructBall* b=nullptr;
    AbstructShirt* s=nullptr;
    b=f->getBall();
    s=f->getshirt();
}