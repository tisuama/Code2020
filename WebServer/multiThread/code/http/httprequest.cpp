#include "httprequest.h"

//不带static，但是const还是要带的
const std::unordered_set<std::string> HttpRequest::DEFAULT_HTML
{
    "/index","/register","/login",
    "/welcome","/video","/picture"
};

const std::unordered_map<std::string,int> HttpRequest::DEFAULT_HTML_TAG
{
    {"/register.html",0},
    {"/login.html",1}
};

//0初始化
void HttpRequest::Init()
{
    Method=Path=Version=Body="";
    state=REQUEST_LINE;
    header.clear();
    post.clear();
}
bool HttpRequest::parse(Buff& buff)
{
    const char  CRLF[]="\r\n";//NEED TEST
    if(buff.ReadableBytes()<=0)
        return false;
    while(buff.ReadableBytes()&&state!=FINISH)
    {
        const char* lineEnd=std::search(buff.Peek(),buff.BeginWriteConst(),CRLF,CRLF+2);
        std::string line(buff.Peek(),lineEnd);
        switch(state)
        {
            case REQUEST_LINE:
                if(!ParseRequestLine(line))
                    return  false;
                ParsePath();
                break;
            case HEADERS:
                ParseHeader(line);
                if(buff.ReadableBytes()<=2)//这个牛逼呀，把post情况排除
                    state=FINISH;
                break;
            case BODY:
                ParseBody(line);
                break;
            default:
                break;
        }
        if(lineEnd==buff.BeginWrite()) 
            break;
        buff.RetrieveUntil(lineEnd+2);    
    }
    LOG_DEBUG("[%s], [%s], [%s]",Method.data(),Path.data(),Version.data());
    return true;
}
std::string HttpRequest::path() const
{
    return Path;
}
std::string& HttpRequest::path()
{
    return Path;
}
std::string HttpRequest::method() const
{
    return Method;
}
std::string HttpRequest::version() const
{
    return Version;
}
std::string HttpRequest::GetPost(const std::string& key) const
{
    assert(key!="");
    if(post.count(key)==1)
        return post.find(key)->second;
    return "";
}
std::string HttpRequest::GetPost(const char* key) const
{
    assert(key!="");
    if(post.count(key)==1)
        return post.find(key)->second;
    return "";
}
bool HttpRequest::IsKeepAlive() const
{
    if(post.count("Connection")==1)
        return post.find("Connection")->second=="Keep-Alive"&&Version=="1.1";
    return false;
}

bool HttpRequest::ParseRequestLine(const std::string& line)//请求行
{
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");//[^ ]非空字符
    std::smatch result;
    if(std::regex_match(line,result,patten))//true,false
    {
        Method=result[1];
        Path=result[2];
        Version=result[3];
        state=HEADERS;
        return true;
    }
    LOG_ERROR("RequestLine Error");
    return false;
}
void HttpRequest::ParseHeader(const std::string& line)
{   
    std::regex patten("^([^:]*): ?(.*)$");
    std::smatch result;
    if(std::regex_match(line,result,patten))
        header[result[1]]=result[2];
    else 
        state=BODY;
}
void HttpRequest::ParseBody(const std::string& line)
{
    Body=line;
    ParsePost();
    state=FINISH;
}
void HttpRequest::ParsePath()
{
    if(Path=="/")
        Path="/index.html";
    else
    {
        for(auto& x:DEFAULT_HTML)
        {
            if(x==Path)
            {
                Path+=".html";
                break;
            }
        }
    }
}
void HttpRequest::ParsePost()
{
    if(Method=="POST"&&header["Content-Type"]=="application/x-www-form-urlencoded")
    {
        ParseFromUrlencoded();
        if(DEFAULT_HTML_TAG.count(Path))
        {
            int tag=DEFAULT_HTML_TAG.find(Path)->second;
            if(tag==0||tag==1)
            {
                bool isLogin=(tag==1);
                if(UserVertify(post["username"],post["password"],isLogin))
                    Path="/welcome.html";
                else
                    Path="/error.html";
            }
        }
    }
}
int HttpRequest::ConverHex(char ch) 
{
    if(ch>='A'&&ch<='F') return ch-'A'+10;
    else if(ch>='a'&&ch<='f') return ch-'a'+10;
    return ch;
}
void HttpRequest::ParseFromUrlencoded()
{
    if(Body.size()==0) return ;
    std::string key,value;
    int num=0;
    int n=Body.size();
    int i,j;
    for(i=0,j=0;i<n;i++)//+->' ' 特殊符号转ASCII hex
    {
        char ch=Body[i];
        if(ch=='=')
        {
            key=Body.substr(j,i-j);
            j=i+1;
        }
        else if(ch=='+')
            Body[i]=' ';
        else if(ch=='%')
        {
            num=ConverHex(Body[i+1])*16+ConverHex(Body[i+2]);
            Body[i+2]=num%10+'0';//解码成10进制
            Body[i+1]=num/10+'0';
            i+=2;
        }
        else if(ch=='&')
        {
            value=Body.substr(j,i-j);
            j=i+1;
            post[key]=value;
            LOG_DEBUG("%s = %s",key.data(),value.data());
        }
    }    
    if(post.count(key)==0&&j<i)
    {
        value=Body.substr(j,i-j);
        post[key]=value;
        LOG_DEBUG("%s = %s",key.data(),value.data());
    }
}
bool HttpRequest::UserVertify(const std::string& name,const std::string& pwd,bool isLogin)
{
    if(name==""||pwd=="") 
        return false;
    LOG_INFO("Vertify name:%s pwd:%s",name.data(),pwd.data());
    MYSQL *sql=nullptr;
    sqlconnRAII(&sql,ConnectionPool::GetInstance());
    assert(sql);
    

    unsigned int j=0;
    char order[256]={0};
    MYSQL_FIELD *fields=nullptr;
    MYSQL_RES *res=nullptr;
    
    snprintf(order,256-1,"SELECT username,password FROM user where username='%s' LIMIT 1",name.data());
    LOG_DEBUG("%s",order);

    if(mysql_query(sql,order))//查询成功返回0
    {
        mysql_free_result(res);
        return false;
    }
    res=mysql_store_result(sql);//查询结果
    j=mysql_num_fields(res);//列
    fields=mysql_fetch_fields(res);//列名数组

    bool register_sucess=true;

    while(MYSQL_ROW row=mysql_fetch_row(res))//从结果集合取一行
    {
        LOG_DEBUG("MYSQL ROW: %s %s",row[0],row[1]);
        std::string password(row[1]);
        if(isLogin) //登陆
        {
            if(pwd==password)
                return true;
            else
            {
                LOG_DEBUG("Login pwd error!");
                return false;
            }
        }
        else
            register_sucess=false;
    }
    mysql_free_result(res);
    
    if(register_sucess)
    {
        bzero(order,256);
        snprintf(order,256-1,"INSERT INTO user(username,password) VALUES('%s','%s')",name.data(),pwd.data());
        LOG_DEBUG("%s",order);
        if(mysql_query(sql,order))
        {
            LOG_DEBUG("Inster error!");
            register_sucess=false;
        }
    }
    ConnectionPool::GetInstance()->ReleaseConnection(sql);
    LOG_DEBUG("UserVertify sucess!");
    return register_sucess;
}
