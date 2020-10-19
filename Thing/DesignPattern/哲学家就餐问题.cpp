
#define N 5
semaphore fork[5];//初值都为1

//方案一 同时拿起左手边叉子 语句阻塞再P(fork[i+1])
void smart_person(int i)
{
    while(true)
    {
        think();
        P(fork[i]);
        P(fork[(i+1)%N]);
        //eat();
        V(fork[i]);
        V(fork[(i+1)%N]);
    }
}

//方案二
#define N 5
semaphore fork[5];//初值都为1
semaphore mutex=1;
void smart_person1()
{
    while(true)
    {
        think();
        P(mutex);
        P(fork[i]);
        P(fork[(i+1)%N]);
        //eat();
        V(fork[i]);
        V(fork[(i+1)%N]);
        V(mutex);
    }
}

//方案三
#define N 5
semaphore fork[5];//初值都为1
void smart_person3
{
    while(true)
    {
        think();
        if(i%2==0)
        {
            P(fork[i]);
            P(fork[i+1]%N);
        }
        else
        {
            P(fork[(i+1)%N]);
            P(fork[i]);
        }
        //eat()
        V(fork[i]);
        V(fork[(i+1)%N]);
    }
}

//方案四