#include "buffer.h"

Buff::Buff(int initBufferSize):buffer_(initBufferSize),readPos_(0),writePos_(0){};

size_t Buff::WriteableBytes() const
{
    return buffer_.size()-writePos_;
}
size_t Buff::ReadableBytes() const
{
    return writePos_-readPos_;
}
size_t Buff::PrependableBytes() const
{
    return readPos_;
}

const char* Buff::Peek() const
{
    return BeginPtr_()+readPos_;
}
void Buff::EnsureWriteable(size_t len)
{
    if(WriteableBytes()<len)
        MakeSpace_(len);
    assert(WriteableBytes()>=len);
}
void Buff::HashWritten(size_t len)
{
    assert(writePos_+len<buffer_.size());
    writePos_+=len;
}

void Buff::Retrieve(size_t len)
{
    assert(len<=ReadableBytes());
    readPos_+=len;
}
void Buff::RetrieveUntil(const char* end)
{
    assert(Peek()<=end);
    Retrieve(end-Peek());
}

void Buff::RetrieveAll()
{
    bzero(&buffer_[0],buffer_.size());
    readPos_=0;
    writePos_=0;
}
std::string Buff::RetrieveAllToStr()
{
    std::string str(Peek(),ReadableBytes());
    RetrieveAll();
    return str;
}
const char* Buff::BeginWriteConst() const
{
    return BeginPtr_()+writePos_;
}
char* Buff::BeginWrite()
{
    return BeginPtr_()+writePos_;
}
void Buff::Append(const std::string&  str)
{
    Append(str.data(),str.length());//str.data()==str.c_str()
}
void Buff::Append(const char* str,size_t len)
{
    assert(str);
    EnsureWriteable(len);
    std::copy(str,str+len,BeginWrite());
    HashWritten(len);
}
void Buff::Append(const void* data,size_t len)
{
    assert(data);
    Append(static_cast<const char*>(data),len);
}
void Buff::Append(const Buff& buff)
{
    Append(buff.Peek(),buff.ReadableBytes());
}
ssize_t Buff::readFd(int fd,int* Errno)//从fd读
{
    char buff[65535];
    struct iovec iov[2];
    const size_t writeSize=WriteableBytes();
    //分散读
    iov[0].iov_base=BeginPtr_()+writePos_;
    iov[0].iov_len=writeSize;
    iov[1].iov_base=buff;
    iov[1].iov_len=sizeof(buff);

    const ssize_t len=readv(fd,iov,2);
    if(len<0)
        *Errno=errno;
    else if(static_cast<size_t>(len)<=writeSize)
        writePos_+=len;
    else
    {
        writePos_+=writeSize;
        Append(buff,len-writeSize);
    }
    return len;
}
ssize_t Buff::writeFd(int fd,int* Errno)//写到fd 可能发生错误，没读
{
    size_t readSize=ReadableBytes();
    ssize_t len=write(fd,Peek(),readSize);
    if(len<0)
    {
        *Errno=errno;
        return len;
    }
    readPos_+=len;
    return len;
}
char* Buff::BeginPtr_()
{
    return &*buffer_.begin();
}
const char* Buff::BeginPtr_() const
{
    return &*buffer_.begin();
}

void Buff::MakeSpace_(size_t len)
{
    if(WriteableBytes()+PrependableBytes()<len)
        buffer_.resize(writePos_+len+1);
    else 
    {
        size_t readSize=ReadableBytes();
        std::copy(BeginPtr_()+readPos_,BeginPtr_()+writePos_,BeginPtr_());
        readPos_=0;
        writePos_=readSize;
        assert(readSize==ReadableBytes());//没啥意义
    }
}