#define N 100
semaphore mutex=1;
semaphore empty=N;
semaphore full=0;

//生产者
void producer()
{
    while(true)
    {
        P(empty);
        P(mutex);
        //生产数据
        V(mutex);
        V(full);
    }
}

//消费者
void consume()
{
    while(true)
    {
        P(full);
        P(mutex);
        //消耗数据
        V(mutex);
        V(empty);
    }
}
