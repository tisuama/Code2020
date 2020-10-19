#include<iostream>
using namespace std;

char str[10]={1};//数组不能赋值 只能初始化
char str1[]={"abc"};
char str2[4]={"abc"};

char *str3="abc";
char str4[]="bac";


void fun(char s[])
{
    cout<<sizeof(s)<<endl;
    cout<<s[1]<<endl;
}
int main()
{
    cout<<strlen(str)<<endl;
    cout<<sizeof(str)<<endl;

    cout<<"---------"<<endl;

    cout<<strlen(str1)<<endl;
    cout<<sizeof(str1)<<endl;   //带'\0'

    cout<<"---------"<<endl;

    cout<<str2[2]<<endl;
    str2[1]='\0';
    cout<<strlen(str)<<endl;

    cout<<"---------"<<endl;

    cout<<sizeof(*str3)<<endl;
    cout<<sizeof(str3)<<endl;
    cout<<sizeof(str4)<<endl;
    cout<<sizeof(*str4)<<endl;

    cout<<"---------"<<endl;


    char *str5=str4;//char* str5=&str4;
    cout<<sizeof(str5)<<endl;

    cout<<"---------"<<endl;

    fun(str4);

    cout<<"---------"<<endl;
    
    char *s="zzzz";
    fun(s);
    
}