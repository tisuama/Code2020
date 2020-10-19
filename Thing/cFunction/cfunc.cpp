#include<iostream>
using namespace std;

char* strcpy(char* dest,const char* src)
{
    if(dest==NULL||src==NULL)
        return NULL;
    if(dest==src) return dest;
    char* d=dest;
    while((*dest++=*src++)!='\0');
    return d;
}
char* strnpy(char* dest,const char* src,size_t count)
{
    if(dest==NULL||src==NULL)
        return NULL;
    char* d=dest;
    while(count&&(*dest++=*src++)!='\0') 
        count--;
    if(count)
        while(count--)
            *dest++='\0';
    return d;
}
void* memcpy(void* dest,const void* src,size_t count)
{
    if(dest==NULL||src==NULL)
        return NULL;
    char* d=(char*)dest;
    char* s=(char*)src;
    while(count--)
        *d++=*s++;
    return dest;
}
void* memmove(void* dest,const void* src,size_t n)
{
    if(dest==NULL||src==NULL)
        return NULL;
    char* d=(char*)dest;
    char* s=(char*)src;
    if(d<=s)
    {
        while(n--)
            *d++=*s++;
    }
    else
    {
        d+=n-1;
        s+=n-1;
        while(n--)
            *d--=*s--;
    }
    return dest;
}

int main()
{
    char* s1="abc";
    char s2[4];
    char s3[4]={"abc"};

    //s2=s1;
    //memcpy(s2,s1,3);
    strcpy(s2,s1);

    cout<<strlen(s1)<<endl;    
    cout<<"address s1 "<<&s1<<" "<<s1<<endl;
    cout<<"address s2 "<<&s2<<" "<<s2<<endl;

    // cout<<sizeof(s3)<<endl;
    // cout<<strlen(s3)<<endl;
    // //cout<<strlen("abc")<<endl;


    cout<<sizeof(s2)<<endl;
    cout<<strlen(s2)<<endl;//奇幻现象
    cout<<s2<<endl;//越界了
}