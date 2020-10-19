#ifndef HTTPREQUEST__H
#define HTTPREQUEST__H

#include <mysql/mysql.h>  //mysql
#include "../buffer/buffer.h"
#include "../pool/sqlconnRAII.h"
#include "../cpplang.h"
#include "../log/log.h"


class HttpRequest{
public:
    HttpRequest(){Init();}
    ~HttpRequest()=default;

    void Init();
    bool parse(Buff& buff);
    std::string path() const;
    std::string& path();

    std::string method() const;
    std::string version() const;

    std::string GetPost(const std::string& key) const;
    std::string GetPost(const char* key) const;

    bool IsKeepAlive() const;
private:
    bool ParseRequestLine(const std::string& line);
    void ParseHeader(const std::string& line);
    void ParseBody(const std::string& line);
    int ConverHex(char ch);
    void ParsePath();
    void ParsePost();
    void ParseFromUrlencoded();
    static bool UserVertify(const std::string& name,const std::string& pwd,bool isLogin);

public:
    enum PARSE_STATE
    {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };
    enum HTTP_CODE
    {
        NO_REQUEST=0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBBIDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_REQUEST,
        CLOSE_CONNECTION,
    };
private:
    PARSE_STATE state;
    std::string Method,Path,Version,Body;
    std::unordered_map<std::string,std::string> header;
    std::unordered_map<std::string,std::string> post;


    static const std::unordered_set<std::string> DEFAULT_HTML;
    static const std::unordered_map<std::string,int> DEFAULT_HTML_TAG;

};



#endif