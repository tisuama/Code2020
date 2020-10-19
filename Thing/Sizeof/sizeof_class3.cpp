
#include<iostream>
using namespace std;
 
class MyA  //大小为4 VS
{
public:
	int a;
};
class MyB :virtual public MyA  //大小为12，变量a,b共8字节，虚基类表指针4 VS
{
public:
	int b;
};
class MyC :virtual public MyA //与B一样12 VS
{
public:
	int c;
};
class MyD :public MyB, public MyC //24,变量a,b,c,d共16(只有一个A)，B的虚基类指针4，C的虚基类指针4 VS
{
public:
	int d;
};
 
int main()
{
	MyA a;
	MyB b;
	MyC c;
	MyD d;
	cout << sizeof(a) << endl;
	cout << sizeof(b) << endl;
	cout << sizeof(c) << endl;
	cout << sizeof(d) << endl;


	return 0;
}



/* VS
class MyB       size(12):
        +---
 0      | {vbptr}
 4      | b
        +---
        +--- (virtual base MyA)
 8      | a
        +---

MyB::$vbtable@:
 0      | 0
 1      | 8 (MyBd(MyB+0)MyA)
 */

/*
class MyC       size(12):
        +---
 0      | {vbptr}
 4      | c
        +---
        +--- (virtual base MyA)
 8      | a
        +---

MyC::$vbtable@:
 0      | 0
 1      | 8 (MyCd(MyC+0)MyA)
*/

/*
class MyD       size(24):
        +---
 0      | +--- (base class MyB)
 0      | | {vbptr}
 4      | | b
        | +---
 8      | +--- (base class MyC)
 8      | | {vbptr}
12      | | c
        | +---
16      | d
        +---
        +--- (virtual base MyA)
20      | a
        +---

MyD::$vbtable@MyB@:
 0      | 0
 1      | 20 (MyDd(MyB+0)MyA)

MyD::$vbtable@MyC@:
 0      | 0
 1      | 12 (MyDd(MyC+0)MyA)
 */

