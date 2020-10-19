#include<iostream>
using namespace std;

//目标抽象类
class Controller
{
public:
    Controller(){}
    virtual void pathPlanning()=0;
};

class DxfPaser
{
public:
    DxfPaser(){};
    void parseFile()
    {
        cout<<"parse file"<<endl;
    }
};

class PathPlanner
{
public:
    PathPlanner(){}
    void calculate()
    {
        cout<<"calculate path"<<endl;
    }
};
class Adapter:public Controller
{
public:
    Adapter()
    {
        dxfPaser=new DxfPaser();
        pathplan=new PathPlanner();
    }
    void pathPlanning()
    {
        cout<<"path planing"<<endl;
        dxfPaser->parseFile();
        pathplan->calculate();
    }
private:
    DxfPaser* dxfPaser;
    PathPlanner* pathplan;
};


int main()
{
    Controller* col=new Adapter();
    col->pathPlanning();
}