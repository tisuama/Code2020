#ifndef THREADPOOL__H
#define THREADPOOL__H

#include "../cpplang.h"
#include<functional>


//magic method
class ThreadPool{
public:
    explicit ThreadPool(int thread_num=6):pool_ptr(std::make_shared<Pool>())
    {
        assert(thread_num>0);
        for(int i=0;i<thread_num;i++)
        {
            std::thread([pool=pool_ptr] //值捕获
            {
                std::unique_lock<std::mutex> lock(pool->mtx); 
                while(true)
                {
                    if(!pool->tasks.empty())
                    {
                        auto task=std::move(pool->tasks.front());
                        pool->tasks.pop();
                        lock.unlock();
                        task();
                        lock.lock();
                    }
                    else if(pool->is_Close)
                        break;
                    else
                        pool->cond.wait(lock);
                }
            }).detach();
        }
    }

    ThreadPool()=default;
    ThreadPool(ThreadPool&&)=default;
    ~ThreadPool()
    {
        if(static_cast<bool>(pool_ptr))
        {
            {
                std::lock_guard<std::mutex> guard(pool_ptr->mtx);
                pool_ptr->is_Close=true;
            }
            pool_ptr->cond.notify_all();
        }
    }
    template<typename F>
    void AddTask(F&& task)
    {
        {//减小loack_guard作用域
            std::lock_guard<std::mutex> guard(pool_ptr->mtx);
            pool_ptr->tasks.emplace(std::forward<F>(task));
        }
        pool_ptr->cond.notify_one();
    }

private:
    struct Pool
    {
        std::mutex mtx;
        std::condition_variable cond;
        bool is_Close;
        std::queue<std::function<void()>> tasks;
    };
    std::shared_ptr<Pool> pool_ptr;
};

// //ThreadPool Test 
// void f()
// {
//     std::cout<<"thread id # "<<std::this_thread::get_id()<<std::endl;
// }
// int main()
// {
//     ThreadPool th_pool(4);
//     std::function<void()> F=f;
//     for(int i=0;i<4;i++)
//         th_pool.AddTask(F);
    
//     std::this_thread::sleep_for(std::chrono::seconds(10));
// }

#endif