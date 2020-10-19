semaphore wmutex=1;//控制写操作的信号量
semaphore rcountmutex=1;//控制对count的修改
int rcount=0;

//方案一 读优先
void writer()
{
    while(true)
    {
        P(wmutex);
        //write();
        V(wmutex);
    }
}
void reader()
{
    while(true)
    {
        P(rcountmutex);//对count的修改互斥
        if(rcount==0)
            P(wmutex);//阻塞写者
        rcount++;
        V(rcountmutex);

        //read

        P(rcountmutex);
        rcount--;
        if(rcount==0)
            V(wmutex);//唤醒写者
        V(rcountmutex);
    }
}


//方案二 写者优先
semaphore rcountmutex=1;
semaphore rmutex=1; //控制读操作的信号量
semaphore wcountmutex=1;
semaphore wmutex=1;//控制写操作的信号量

int wcount=0;
int rcount=0;

void write()
{
    while(true)
    {
        P(wcountmutex);
        if(wcount==0)
            P(rmutex);
        wcount++;
        V(wcountmutex);

        P(wmutex);
        //write
        V(wmutex);

        P(wcountmutex);
        wcount--;
        if(wcount==0)
            V(rmutex);
        V(wcountmutex);

    }
}

void read()
{
    while(true)
    {
        P(rmutex);
        P(rcountmutex);
        if(rcount==0)
            P(wmutex);
        rcount++;
        V(rcountmutex);
        V(rmutex);

        //read

        P(rcountmutex);
        rcount--;
        if(rcount==0)
            V(wmutex);
        V(rcountmutex);
    }
}

//方案三 公平算法

semaphore rcountmutex=1;//控制对rcount的修改
semaphore wmutex=1;//控制写操作
semaphore flag=1;//实现公平竞争
int rcount=0;

void writer()
{
    while(true)
    {
        P(flag);
        P(wmutex);
        //write()
        V(wmutex);
        V(flag);
    }
}

void reader()
{
    while(true)
    {
        P(flag);
        P(rcountmutex);
        if(rcount==0)
            P(wmutex);
        rcount++;
        V(rcountmutex);
        V(flag);

        //read

        P(rcountmutex);
        rcount--;
        if(rount==0)
            V(wmutex);
        V(rcountmutex);
    }
}