#include "log.h"

void Log::init(int level_,const char* path_,
            const char* suffix_,
            int maxQueueCapacity_)
{
    isOpen=true;
    level=level_;
    if(maxQueueCapacity_>0)
    {
        isAsync=true;
        if(!q)
        {
            //change
            std::unique_ptr<BlockDeque<std::string>> newDeque(new BlockDeque<std::string>);
            q=std::move(newDeque);
            std::unique_ptr<std::thread> NewThread(new std::thread(FlushLogThread));
            writeThread=std::move(NewThread);
        }
    }
    else  
        isAsync=false;
    lineCount=0;
    std::time_t timer=std::time(nullptr);
    std::tm* sysTime=std::localtime(&timer);
    std::tm t=*sysTime;
    path=path_;
    suffix=suffix_;
    char fileName[lOG_NAME_LEN]={0};
    snprintf(fileName,lOG_NAME_LEN-1,"%s/%04d_%02d_%02d%s",path,t.tm_year+1900,t.tm_mon+1,t.tm_mday,suffix);
    toDay=t.tm_mday;
    {
        std::lock_guard<std::mutex> guard(mtx);
        buff.RetrieveAll();
        if(fp)
        {
            fflush(fp);
            fclose(fp);
        }
        fp=fopen(fileName,"a");
        if(fp==nullptr)
        {
            mkdir(path,0777);
            fp=fopen(fileName,"a");
        }
        assert(fp!=nullptr);
    }
}
Log* Log::Instance()
{
    static Log log;
    return &log;
}
void Log::FlushLogThread()
{
    Log::Instance()->AsyncWrite();
}
void Log::write(int level_,const char* format,...)
{
    //获取时间Linux
    struct timeval now{0,0};
    gettimeofday(&now,nullptr);//now.tv_sec time_t类型
    std::time_t tSec=now.tv_sec;
    struct tm t;
    localtime_r(&tSec,&t);

    va_list vaList;
    if(toDay!=t.tm_mday||(lineCount&&(lineCount%MAX_LINES==0)))
    {
        //change
        std::unique_lock<std::mutex> locker(mtx);
        locker.unlock();
        char newFile[lOG_NAME_LEN];
        char tail[36]{0};
        snprintf(tail,36-1,"%04d_%02d_%02d",t.tm_year+1900,t.tm_mon+1,t.tm_mday);

        if(toDay!=t.tm_mday)
        {
            snprintf(newFile,lOG_NAME_LEN-72,"%s/%s%s",path,tail,suffix);
            toDay=t.tm_mday;
            lineCount=0;
        }
        else
            snprintf(newFile,lOG_NAME_LEN-72,"%s/%s-%d%s",path,tail,(lineCount/MAX_LINES),suffix);
        locker.lock();
        fflush(fp);
        fclose(fp);
        fp=fopen(newFile,"a"); 
        assert(fp!=nullptr);
    }
    {
        std::unique_lock<std::mutex> locker(mtx);
        lineCount++;
        int n=snprintf(buff.BeginWrite(),128,"%d-%02d-%02d %02d:%02d:%02d ",
                        t.tm_year+1900,t.tm_mon+1,t.tm_mday,
                        t.tm_hour,t.tm_min,t.tm_sec);  
        buff.HashWritten(n);
        AppendLogLevelTitle(level_);
        
        va_start(vaList,format);
        int m=vsnprintf(buff.BeginWrite(),buff.WriteableBytes(),format,vaList);//可变参数打印数据
        va_end(vaList);

        buff.HashWritten(m);
        buff.Append("\n\0",2);

        if(isAsync&&q&&!q->full())
            q->push_back(buff.RetrieveAllToStr());
        else
            fputs(buff.Peek(),fp);//写到fp,不包括空字符
        buff.RetrieveAll();
    }
}
void Log::flush()
{
    {
        std::lock_guard<std::mutex> guard(mtx);
        fflush(fp);
    }
    if(isAsync)
        q->flush();
}
int Log::GetLevel()
{
    std::lock_guard<std::mutex> guard(mtx);
    return level;
}
void Log::SetLevel(int level_)
{
    std::lock_guard<std::mutex> guard(mtx);
    level=level_;
}
Log::Log()
{
    lineCount=0;
    isAsync=false;
    writeThread=nullptr;
    q=nullptr;
    toDay=0;
    fp=nullptr;
}
Log::~Log()//no virtual
{
    if(writeThread&&writeThread->joinable())
    {
        while(!q->empty())
            q->flush();
        q->Close();
        writeThread->join();//等待运行结束
    }
    if(fp)
    {
        std::lock_guard<std::mutex> guard(mtx);
        flush();
        fclose(fp);
    }
}
void Log::AppendLogLevelTitle(int level_)
{
    switch(level_)
    {
        case 0:
            buff.Append("[debug]: ",9);
            break;
        case 1:
            buff.Append("[info] : ",9);
            break;
        case 2:
            buff.Append("[warn] : ",9);
            break;
        case 3:
            buff.Append("[error]: ",9);
            break;
        default:
            buff.Append("[info] : ",9);
            break;
    }
}
void Log::AsyncWrite()
{
    std::string str="";
    while(q->pop(str))
    {
        std::lock_guard<std::mutex> guard(mtx);   
        fputs(str.data(),fp);     
    }
}
