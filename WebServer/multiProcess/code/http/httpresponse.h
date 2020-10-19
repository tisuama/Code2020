#ifndef HTTPRESPONSE__H
#define HTTPRESPONSE__H
#include "../cpplang.h"
#include "../buffer/buffer.h"
#include "../log/log.h"


class HttpResponse{
public:
    HttpResponse();
    ~HttpResponse();
    void Init(const std::string& srcDir_,std::string& path_,bool isKeepAlive_=false,int code_=-1);
    void MakeResponse(Buff& buff);
    void unmapFile();
    char* File();
    size_t FileLen() const;
    void ErrorContent(Buff& buff,std::string message);
    int Code() const {return code;}


private:
    void AddStateLine(Buff &buff);
    void AddHeader(Buff &buff);
    void AddContent(Buff &buff);

    void ErrorHtml();
    std::string GetFileType();

private:
    int code;
    bool isKeepAlive;

    std::string path;
    std::string srcDir;

    char* mmFile;
    struct stat mmFileStat;

    static const std::unordered_map<std::string,std::string> SUFFIX_TYPE;
    static const std::unordered_map<int,std::string> CODE_STATUS;
    static const std::unordered_map<int,std::string> CODE_PATH;
};


#endif