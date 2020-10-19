#include<iostream>
#include<functional>
#include<vector>
#include<list>
#include<algorithm>
using namespace std;

template<class T>
inline auto Add(T x,T y)->decltype(x){return x+y;}

class C
{
public:
    explicit C(const string& s):mystr(s){}
    friend ostream& operator<<(ostream& out,const C& c)
    {
        out<<c.mystr<<endl;
        return out;
    }

private:
    string mystr;
};



int main()
{
    auto lll=[](int x){return x+10;};
    function<int(int)> ll=[](int x){return x+10;};
    cout<<ll(10)<<endl;

    list<int> l{1,2,3};
    list<int>::iterator it;
    it=find(l.begin(),l.end(),10);
    auto it1=find(l.begin(),l.end(),11);

    //不利因素
    vector<bool> v(true);
    auto var=v.at(0);
    cout<<var<<endl;
    cout<<typeid(var).name()<<endl;

    C c("abc");

    vector<C> res{c};
    for(const C& elem:res);
}