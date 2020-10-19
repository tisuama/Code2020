#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

//qsort
template<class T>
void sort(T A[],int l,int r)
{
    if(l>r)
        return ;
    int val=A[l];
    int x=l,y=r;
    while(l<r)
    {
        while(l<r&&A[r]>=val) r--;
        A[l]=A[r];
        while(l<r&&A[l]<=val) l++;
        A[r]=A[l];
    }
    A[l]=val;
    sort(A,x,l-1);
    sort(A,l+1,y);
}

//merge_sort
template<class T>
void merge(T A[],int st,int mid,int ed)
{
    int len1=mid-st+1,len2=ed-mid;
    T* L[len1];
    T* R[len2];
    for(int i=st;i<=mid;i++)
        L[i-st]=A+i;
    for(int i=mid+1;i<=ed;i++)
        R[i-(mid+1)]=A+i;
    int i=0,j=0;
    while(i<len1&&j<len2)
    {
        if(L[i]<R[j])
            A[st++]=*(L+i);
        else
            A[st++]=*(R+j);
        
    }
    if(i<len1)
    {
        for(;i<len1;i++)
            A[st++]=*(L+i);
    }
    if(j<len2)
    {
        for(;j<len2;j++)
            A[st++]=*(R+j);
    }
}

template<class T>
void merge_sort(T A[],int st,int ed)
{
    //cout<<"hehe "<<st<<" "<<ed<<endl;
    if(st<ed)
    {
        int mid=(st+ed)>>1;
        merge_sort(A,st,mid);
        merge_sort(A,mid+1,ed);
        merge(A,st,mid,ed);
    }
}

template<class T>
void Merge_Sort(T A[],int st,int ed)
{
    int len=ed-st+1;
    T tmp[len+1];
    for(int s=2;s<len;s*=2)
    {   
        //cout<<"hehe "<<i<<endl;
        int id=st;
        int l2,r1,r2;
        for(int l1=st;l1<=ed;l1=r2+1)
        {
            l2=l1+s-1;
            r1=l1+s;
            r2=min(r1+s-1,ed);
            //比较  
            int i=l1,j=r1;
            while(i<=l2&&j<=r2)
            {
                if(A[i]<A[j])
                    tmp[id++]=A[i++];
                else
                    tmp[id++]=A[j++];
            }
            while(i<=l2)
                tmp[id++]=A[i++];
            while(j<=r2)
                tmp[id++]=A[j++];
        }
        for(int k=st;k<=ed;k++)
            A[k]=tmp[k];
    }
}

template<class T>
void sort(T A[],int n)
{
    for(int i=0;i<n;i++)
    {
        for(int j=n-1;j>i;j--)
            if(A[j]>A[j-1])
                swap(A[j],A[j-1]);
    }
}
template<class T>
void display(T a[],int n)
{
    for(int i=0;i<n;i++)
        cout<<a[i]<<"\t"<<endl;
}

int main()
{


    int a[]={1,31,2,5,21,23};
    char b[]={'a','x','y','e'};
    sort(a,6);
    sort(b,5);
    display(a,6);
    display(b,5);


    //qsort
    cout<<"qsort=========="<<endl;
    sort(a,0,5);
    display(a,6);


    cout<<"merge_srot========="<<endl;
    Merge_Sort(a,0,5);
    display(a,6);



    cout<<"less============"<<endl;
    vector<int> V;
    for(int i=0;i<10;i++)
        V.push_back(10-i);
    sort(V.begin(),V.end(),less<int>());
    for(int i=0;i<10;i++)
        cout<<V[i]<<endl;
}