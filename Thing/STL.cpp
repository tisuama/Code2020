#include<bits/stdc++.h>
using namespace std;



int main()
{

    //multiset
    multiset<int> S;
    S.insert(1);
    S.insert(-2);
    S.insert(1);
    S.erase(1);
    for(auto& x:S)
        cout<<x<<endl;
    //ultimap
    multimap<int,int> mp;
    mp.insert({1,-1});
    mp.insert({1,3});
    mp.insert({1,2});
    //mp.erase(1);
    cout<<(*mp.find(1)).second<<endl;
    for(auto& x:mp)
        cout<<x.first<<" "<<x.second<<endl;
    
    //vector
    vector<int> V;
    for(int i=1;i<=10;i+=2)
        V.push_back(i);
    int x=lower_bound(V.begin(),V.end(),2)-V.begin();
    cout<<V.end()-V.begin()<<endl;
    cout<<x<<endl;

}