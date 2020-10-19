#include<iostream>
#include<vector>
using namespace std;
typedef size_t size_type;

// void insert(iterator p,const T& x)
// {
//     if(finish!=end_of_storge)//还有空间
//     {
//         construct(finish,*(fiish-1));
//         ++finish;
//         T x_copy=x;
//         copy_backward(p,finish-2,finish-1); 
//         *p=x_copy;
//     }
// }
// void erase(iterator p)
// {
//     if(p+1!=end())//不是最后一个元素
//         copy(p+1,finish,p);
//     --finish;
//     destroy(finish);
// }

size_type max_size()
{
    return size_type(-1)/sizeof(int);
}
int main()
{
    cout<<max_size()<<endl;

    //vector
    vector<int> A;
    for(int i=0;i<10;i++)
        A.push_back(i);
    cout<<*(A.end()-1)<<endl;
}