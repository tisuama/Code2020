#include<iostream>
#include<vector>
using namespace std;
void quick_sort(int x,int y,int A[])
{
    if(x>=y) return;
    int val=A[x];
    int l=x,r=y;
    while(l<r)
    {
        while(l<r&&A[r]>=val) r--;
        A[l]=A[r];
        while(l<r&&A[l]<val) l++;
        A[r]=A[l];
    }
    A[l]=val;
    quick_sort(x,l-1,A);
    quick_sort(l+1,y,A);
}

void merge(int x,int mid,int y,int A[])
{
    vector<int> B,C;
    for(int i=x;i<=mid;i++) B.push_back(A[i]);
    for(int i=mid+1;i<=y;i++) C.push_back(A[i]);
    int l=0,r=0;
    while(l<B.size()&&r<C.size())
    {
        if(B[l]<C[r])
            A[x++]=B[l++];
        else
            A[x++]=C[r++];
    }
    while(l<B.size())
        A[x++]=B[l++];
    while(r<C.size())
        A[x++]=C[r++];
}

void merge_sort(int x,int y,int A[])
{
    if(x<y)
    {
        int mid=(x+y)>>1;
        //cout<<"hehe "<<x<<" "<<y<<" "<<mid<<endl;
        merge_sort(x,mid,A);
        merge_sort(mid+1,y,A);
        merge(x,mid,y,A);
    }
}

void sort(int x,int y,int A[])
{
    for(int i=x;i<=y;i++)
    {
        for(int j=y;j>i;j--)
        {
            if(A[j]>A[j-1])
                swap(A[j],A[j-1]);
        }
    }
}


void show(int A[])
{
    for(int i=0;i<10;i++) cout<<A[i]<<" ";
    cout<<endl;
}
void reset(int A[])
{
    for(int i=0;i<5;i++)
        A[i]=10-i;
    for(int i=9;i>=5;i--)
        A[i]=i;    
}


class heap
{
public:
    void push(int& x)
    {
        A.push_back(x);
        shiftup(A.size()-1);
    }
    void shiftup(int index)
    {
        int i=index;
        int fa=(i-1)/2;
        while(fa>=0)
        {
            if(A[fa]<A[i]) 
                swap(A[fa],A[i]),i=fa;
            else break;
            if(!fa) break;
            fa=(i-1)/2;
        }
    }
    int pop()
    {
        swap(A[0],A[A.size()-1]);
        int x=A.back();
        A.pop_back();
        shiftdown(A.size()-1);
        return x;
    }
    void shiftdown(int index)
    {
        int i=0;
        int l=i*2+1;
        while(l<=index)
        {
            if(l<index&&A[l]<A[l+1])
                l++;
            if(A[i]<A[l]) 
                swap(A[i],A[l]),i=l;
            else break;
            l=i*2+1;
        }
    }
private:
    vector<int> A;//大
};

int main()
{
    int A[10];
    //sort
    reset(A);
    show(A);
    sort(0,9,A);
    show(A);    

    //quick_sort
    reset(A);
    show(A);
    quick_sort(0,9,A);
    show(A);

    //merge_sort
    reset(A);
    show(A);
    merge_sort(0,9,A);
    show(A);    


    //heap
    heap h;
    for(int i=0;i<5;i++) h.push(i);
    for(int i=10;i>3;i--) h.push(i);
    for(int i=0;i<10;i++) cout<<h.pop()<<" ";
}