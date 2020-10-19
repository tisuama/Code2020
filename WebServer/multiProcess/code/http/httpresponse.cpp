#include "httpresponse.h"

const std::unordered_map<std::string,std::string> HttpResponse::SUFFIX_TYPE
{
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".word",  "application/nsword" },
    { ".png",   "image/png" },
    { ".gif",   "image/gif" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".au",    "audio/basic" },
    { ".mpeg",  "video/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".avi",   "video/x-msvideo" },
    { ".gz",    "application/x-gzip" },
    { ".tar",   "application/x-tar" },
    { ".css",   "text/css "},
    { ".js",    "text/javascript "},
};
const std::unordered_map<int,std::string> HttpResponse::CODE_STATUS
{
    {200,"OK"},
    {400,"Bad Request"},
    {403,"Forbidden"},
    {404,"Not Found"},
};
const std::unordered_map<int,std::string> HttpResponse::CODE_PATH
{
    {400,"/400.html"},  
    {403,"/400.html"},    
    {404,"/400.html"},
};

HttpResponse::HttpResponse()
{
    code=-1;
    path=srcDir="";
    isKeepAlive=false;
    mmFile=nullptr;
    mmFileStat={0};//struct | char S[100]={0};
}
HttpResponse::~HttpResponse()
{
    unmapFile();
}
void HttpResponse::Init(const std::string& srcDir_,std::string& path_,bool isKeepAlive_,int code_)
{
    assert(srcDir_!=""&&path_!="");//指定srcDir和path
    if(mmFile)  {unmapFile();}
    code=code_;
    isKeepAlive=isKeepAlive_;
    path=path_;
    srcDir=srcDir_;
    mmFile=nullptr;
    mmFileStat={0};
}
void HttpResponse::MakeResponse(Buff& buff)
{
    if(stat((srcDir+path).data(),&mmFileStat)<0||S_ISDIR(mmFileStat.st_mode))
        code=404;
    else if(!(mmFileStat.st_mode&S_IROTH))
        code=403;
    else if(code==-1)
        code=200;
    ErrorHtml();//根据code的值 更新path
    AddStateLine(buff);
    AddHeader(buff);
    AddContent(buff);
}
void HttpResponse::unmapFile()
{
    if(mmFile)
    {
        munmap(mmFile,mmFileStat.st_size);
        mmFile=nullptr;
    }
}
char* HttpResponse::File()
{
    return mmFile;
} 
size_t HttpResponse::FileLen() const
{
    return mmFileStat.st_size;
}
void HttpResponse::ErrorContent(Buff& buff,std::string message)
{
    std::string body;
    std::string status;
    body+="<html><title>Error</title>\n";
    body+="<body bgcolor=\"ffffff\">\n";
    if(CODE_STATUS.count(code)==1)
        status=CODE_STATUS.find(code)->second;
    else
        status="Bad Request";
    body+=std::to_string(code)+" : "+status+"\n";
    body+="<p>"+message+"</p>";
    body+="<hr><em>WebServer></em></body></html>\n";
    buff.Append("Content-length: "+std::to_string(body.size())+"\r\n\r\n");
    buff.Append(body);
}
void HttpResponse::AddStateLine(Buff &buff)
{
    std::string status;
    if(CODE_STATUS.count(code)==1)
        status=CODE_STATUS.find(code)->second;
    else
    {
        code=400;
        status=CODE_STATUS.find(400)->second;
    }
    buff.Append("HTTP/1.1 "+std::to_string(code)+" "+status+"\r\n");
}
void HttpResponse::AddHeader(Buff &buff)
{   
    buff.Append("Connection: ");
    if(isKeepAlive)
    {
        buff.Append("Keep-Alive\r\n");
        buff.Append("Keep-Alive: timeout=10000\r\n");
    }
    else
        buff.Append("close\r\n");
    buff.Append("Content-type: "+GetFileType()+"\r\n");

}
void HttpResponse::AddContent(Buff &buff)
{
    int srcFd=open((srcDir+path).data(),O_RDONLY);
    if(srcFd<0)
    {
        ErrorContent(buff,"File NotFound!");
        return ;
    }
    LOG_DEBUG("File path %s",(srcDir+path).data());
    int* mmRet=(int*)mmap(0,mmFileStat.st_size,PROT_READ,MAP_PRIVATE,srcFd,0);
    if(*mmRet==-1)
    {
        ErrorContent(buff,"File NotFound!");
        return ;
    }
    mmFile=(char*)mmRet;
    close(srcFd);
    buff.Append("Content-length: "+std::to_string(mmFileStat.st_size)+"\r\n\r\n");

}
void HttpResponse::ErrorHtml()//400系列客户端错误
{
    if(CODE_PATH.count(code)==1)
    {
        path=CODE_PATH.find(code)->second;
        stat((srcDir+path).data(),&mmFileStat);
    }
}
std::string HttpResponse::GetFileType()
{
    size_t idx=path.find_last_of('.');
    if(idx==std::string::npos)
        return "text/pain";
    std::string suffix=path.substr(idx);
    if(SUFFIX_TYPE.count(suffix)==1)
        return SUFFIX_TYPE.find(suffix)->second;
    return "text/pain";
}
