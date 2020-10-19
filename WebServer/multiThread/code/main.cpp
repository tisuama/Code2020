#include<iostream>
#include "./server/webserver.h"



int main()
{
    WebServer server(
        1314,3,5000,false,     //端口 ET模式 timeout 优雅退出
        3306,"root","","yourdb",//mysql配置 port user passwd database
        12,4,false,0,1024  //连接池 线程数 日志开关 日志等级（最小等级） 异步日志队列
    );
    server.Start();
}