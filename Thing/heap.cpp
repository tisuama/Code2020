#include<iostream>
#include<vector>
using namespace std;

class heap
{
public:
    void push(int x)
    {
        h.push_back(x);
        shiftup();
    }
    int pop()
    {
        swap(h[0],h[h.size()-1]);
        int x=h.back();
        h.pop_back();
        shiftdown();
        return x;
    }
    void shiftup()
    {
        int index=h.size()-1;
        int pre=(index-1)/2;
        while(index)
        {
            if(h[index]>h[pre])
                swap(h[index],h[pre]);
            else
                break;
            index=pre;
            pre=(pre-1)/2;
        }
    }
    void shiftdown()
    {
        int index=0;
        int next=(index+1)*2;//右边的
        while(index<=h.size())
        {
            if(index==h.size()||h[next]<h[next-1])//考虑只存在一个左子树
                next--;
            if(h[next]>h[index])
                swap(h[next],h[index]);
            else
                break;
            index=next;
            next=(next+1)*2;
        }
    }

private:
    vector<int> h;
};

int main()
{
    heap H;
    for(int i=10;i>=0;i--)
        H.push(i);
    cout<<H.pop()<<endl;
}