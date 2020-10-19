#include<iostream>
using namespace std;
class A
{
    int a;
    virtual void fun() {}
    virtual void fun1() {}
};
class B
{
    int b;
    virtual void fun2() {}
    virtual void fun3() {}
};
class MyC : virtual public  A, virtual public B
{
    public:
        virtual void fun() {}
};

int main()
{

    cout<<sizeof(A)<<" "<<sizeof(B)<<" "<<sizeof(MyC)<<endl;//8 8 20 VS
    return 0;
}


/*!!!!!!!!
class MyC       size(20):
		+---
 0      | {vbptr}
		+---
		+--- (virtual base A)
 4      | {vfptr}
 8      | a
		+---
		+--- (virtual base B)
12      | {vfptr}
16      | b
		+---

MyC::$vbtable@:
 0      | 0
 1      | 4 (MyCd(MyC+0)A)
 2      | 12 (MyCd(MyC+0)B)

MyC::$vftable@A@:
		| -4
 0      | &MyC::fun
 1      | &A::fun1

MyC::$vftable@B@:
		| -12
 0      | &B::fun2
 1      | &B::fun3
 */

