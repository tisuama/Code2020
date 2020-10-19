#include<iostream>
using namespace std;
const int maxn=1e5+100;
struct Trie
{
    Trie* next[26];
    int cnt;
};
Trie *root;
void insert(char *str)
{
    int len=strlen(str);
    Trie *p=root,*q;
    for(int i=0;i<len;i++)
    {
        int id=str[i]-'a';
        if(p->next[id]==NULL)
        {
            q=new Trie();
            q->cnt=1;//包含的前缀
            p->next[id]=q;
            p=p->next[id];
        }
        else
        {
            p=p->next[id];
            ++p->cnt;
        }
    }
}
int query(char *str)
{
    int len=strlen(str);
    Trie *p=root;
    for(int i=0;i<len;i++)
    {
        int id=str[i];
        p=p->next[id];
        if(p==NULL) return 0;
    }
    return p->cnt;
}

int main()
{
    root=new Trie();
    
}