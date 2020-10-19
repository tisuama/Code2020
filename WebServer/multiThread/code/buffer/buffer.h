#ifndef BUFF__H
#define BUFF__H

#include "../cpplang.h"

class Buff{
public:
    Buff(int initBufferSize=1024);
    ~Buff()=default;

    size_t WriteableBytes() const;
    size_t ReadableBytes()  const;
    size_t PrependableBytes()  const;    

    const char* Peek() const;
    void EnsureWriteable(size_t len);
    void HashWritten(size_t len);

    void Retrieve(size_t len);
    void RetrieveUntil(const char* end);

    void RetrieveAll();
    std::string RetrieveAllToStr();

    const char* BeginWriteConst() const;
    char* BeginWrite();

    void Append(const std::string&  str);
    void Append(const char* str,size_t len);
    void Append(const void* data,size_t len);
    void Append(const Buff& buff);

    ssize_t readFd(int fd,int* Errno);
    ssize_t writeFd(int fd,int* Errno);

private:
    char* BeginPtr_();
    const char* BeginPtr_() const;

    void MakeSpace_(size_t len);
    std::vector<char> buffer_;
    std::atomic<size_t> readPos_;
    std::atomic<size_t> writePos_;
};

#endif