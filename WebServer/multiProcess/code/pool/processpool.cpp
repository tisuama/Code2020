#include "processpool.h"

static int sig_pipefd[2];

static void setnoblocking(int fd)
{
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
}

static void addsig(int sig,void(*handler)(int),bool restart=true)//这里有问题
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler=handler;
    if(restart)
    {
        sa.sa_flags|=SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig,&sa,NULL)!=-1);
}

static void sig_handler(int sig)
{
    int save_errno=errno;
    int msg=sig;
    send(sig_pipefd[1],(char*)msg,1,0);
    errno=save_errno;
}

static void addFd(int epollfd,int fd,uint32_t event)
{
    epoll_event ev;
    ev.data.fd=fd;
    ev.events=event;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
    setnoblocking(fd);
}
static void removeFd(int epollfd,int fd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}
//setnoblocking,addfd,removefd
void processpool::setup_sig_pipe()
{
    pool_ptr->m_epoll=epoll_create(5);
    assert(pool_ptr->m_epoll!=-1);   

    int ret=socketpair(PF_UNIX,SOCK_STREAM,0,sig_pipefd);
    assert(ret!=-1);
    
    setnoblocking(sig_pipefd[1]);


    uint32_t events=EPOLLIN|EPOLLET;
    addFd(pool_ptr->m_epoll,sig_pipefd[0],events);

    //设置信号处理函数
    addsig(SIGCHLD,sig_handler);
    addsig(SIGTERM,sig_handler);
    addsig(SIGINT,sig_handler);
    addsig(SIGPIPE,SIG_IGN);
    
}

processpool* processpool::Instance()
{
    static processpool pool;
    return &pool;
}

void processpool::run()
{
    if(pool_ptr->m_idx!=-1)
    {
        run_child();
        return ;
    }
    run_parent();
}
void processpool::init(int listenfd,int process_number)
{
    pool_ptr=std::make_shared<pool>();
    pool_ptr->m_listenfd=listenfd;
    pool_ptr->m_process_num=process_number;
    pool_ptr->m_idx=-1;
    pool_ptr->m_stop=false;

    assert((process_number>0&&process_number<=MAX_PROCESS_NUMBER));

    pool_ptr->m_sub_process=new process[process_number];
    assert(pool_ptr->m_sub_process);

    for(int i=0;i<process_number;i++)
    {
        int ret=socketpair(PF_UNIX,SOCK_STREAM,0,pool_ptr->m_sub_process[i].m_pipefd);
        assert(ret==0);

        pool_ptr->m_sub_process[i].m_pid=fork();
        if(pool_ptr->m_sub_process[i].m_pid>0)//父进程
        {
            close(pool_ptr->m_sub_process[i].m_pipefd[1]);
        }
        else//子进程
        {
            close(pool_ptr->m_sub_process[i].m_pipefd[0]);
            pool_ptr->m_idx=i;
            break;
        }
    }

}
void processpool::run_parent()
{
    setup_sig_pipe();//fork之后epoll_crete

    uint32_t events=EPOLLIN|EPOLLET;
//    addFd(pool_ptr->m_epoll,pool_ptr->m_listenfd,events);//父进程监听listenfd

    epoll_event ev[MAX_EVENT_NUMBER];
    int sub_process_counter=0;
    int new_conn=1;
    int number=0;
    int ret=-1;
    while(!pool_ptr->m_stop)
    {  
        number=epoll_wait(pool_ptr->m_epoll,ev,MAX_EVENT_NUMBER,-1);
        if(number<0&&errno!=EINTR)
        {
            LOG_ERROR("parent epoll failure");
            break;
        }
        for(int i=0;i<number;i++)
        {
            int sockfd=ev[i].data.fd;
            if(sockfd==pool_ptr->m_listenfd)
            {
                int id=sub_process_counter;
                //Round Robin
                do
                {
                    if(pool_ptr->m_sub_process[id].m_pid!=-1)
                        break;
                    id=(id+1)%pool_ptr->m_process_num;

                } while (id!=sub_process_counter);
                if(pool_ptr->m_sub_process[id].m_pid==-1)
                {
                    pool_ptr->m_stop=true;
                    break;
                }
                sub_process_counter=(sub_process_counter+1)%pool_ptr->m_process_num;
                send(pool_ptr->m_sub_process[id].m_pipefd[0],(char*)&new_conn,sizeof(new_conn),0);
                LOG_INFO("send request to child");
            }
            else if((sockfd==sig_pipefd[0])&&(ev[i].events&EPOLLIN))
            {
                char signals[1024];
                ret=recv(sig_pipefd[0],signals,sizeof(signals),0);
                if(ret<0)
                    continue;
                else
                {
                    for(int j=0;j<ret;j++)
                    {   
                        switch (signals[j])
                        {
                            case SIGCHLD:
                            {
                                pid_t pid;
                                int stat;
                                while((pid==waitpid(-1,&stat,WNOHANG))>0)
                                {
                                    for(int k=0;k<pool_ptr->m_process_num;k++)
                                    {
                                        if(pool_ptr->m_sub_process[k].m_pid==pid)
                                        {
                                            LOG_INFO("child join");
                                            close(pool_ptr->m_sub_process[k].m_pipefd[0]);
                                            pool_ptr->m_sub_process[k].m_pid=-1;
                                        }
                                    }
                                }
                                pool_ptr->m_stop=true;
                                for(int k=0;k<pool_ptr->m_process_num;k++)
                                {
                                    if(pool_ptr->m_sub_process[k].m_pid!=-1)
                                        pool_ptr->m_stop=false;
                                }
                                break;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                                LOG_INFO("kill all the child");
                                for(int k=0;k<pool_ptr->m_process_num;k++)
                                {
                                    int pid=pool_ptr->m_sub_process[k].m_pid;
                                    if(pid!=-1)
                                        kill(pid,SIGTERM);
                                }
                                break;
                            }
                            default:
                                break;
                        }
                    }   
                }
            }
            else
                continue;
        }
    }

}
void processpool::run_child()
{
    setup_sig_pipe();

    int pipefd=pool_ptr->m_sub_process[pool_ptr->m_idx].m_pipefd[1];

    uint32_t events=EPOLLIN|EPOLLET;
    addFd(pool_ptr->m_epoll,pipefd,events);   
    addFd(pool_ptr->m_epoll,pool_ptr->m_listenfd,events);

    epoll_event ev[MAX_EVENT_NUMBER];
    std::unordered_map<int,HttpConn> users;
    int number=0;
    int ret=-1;

    while(!pool_ptr->m_stop)
    {
        number=epoll_wait(pool_ptr->m_epoll,ev,MAX_EVENT_NUMBER,-1);
        LOG_INFO("child %d has epoll num %d",pool_ptr->m_idx,number);
        if((number<0)&&(errno!=EINTR))
        {
            LOG_ERROR("child epoll failure");
            break;
        }
        for(int i=0;i<number;i++)
        {
            int sockfd=ev[i].data.fd;
            if(sockfd==pipefd&&(ev[i].events&EPOLLIN))
            {
                int client=0;
                ret=recv(sockfd,(char*)&client,sizeof(client),0);
                if(((ret<0)&&(errno!=EAGAIN))||ret==0)
                    continue;
                else
                {
                    struct sockaddr_in client_address;
                    socklen_t len=sizeof(client_address);
                    int connfd=accept(pool_ptr->m_listenfd,(struct sockaddr*)&client_address,&len);
                    if(connfd<0) 
                    {
                        LOG_ERROR("errno is %d",errno);
                        continue;
                    }
                    addFd(pool_ptr->m_epoll,connfd,events);
                    users[connfd].init(connfd,client_address);
                }
            }
            else if(sockfd==pool_ptr->m_listenfd)
            {
                    struct sockaddr_in client_address;
                    socklen_t len=sizeof(client_address);
                    int connfd=accept(pool_ptr->m_listenfd,(struct sockaddr*)&client_address,&len);
                    if(connfd<0) 
                    {
                        LOG_ERROR("errno is %d",errno);
                        continue;
                    }
                    addFd(pool_ptr->m_epoll,connfd,events);
                    users[connfd].init(connfd,client_address);                
            }
            else if((sockfd==sig_pipefd[0])&&(ev[i].events&EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret=recv(sig_pipefd[0],signals,sizeof(signals),0);
                if(ret<=0)
                    continue;
                else
                {
                    for(int j=0;j<ret;j++)
                    {
                        switch(signals[j])
                        {
                            case SIGCHLD:
                            {
                                pid_t pid;
                                int stat;
                                while((pid==waitpid(-1,&stat,WNOHANG))>0)
                                    continue;
                                break;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                                pool_ptr->m_stop=true;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }
            else if(ev[i].events&EPOLLIN)
                OnRead(&users[sockfd]);
            else if(ev[i].events&EPOLLOUT)
                OnWrite(&users[sockfd]);
            else
                continue;
        }
    }
    users.clear();
    close(pipefd);
}

void processpool::OnRead(HttpConn* client)
{
    assert(client);
    int ret=-1;
    int readErrno=0;
    ret=client->read(&readErrno);
    if(ret<=0&&readErrno!=EAGAIN)
    {
        CloseConn(client);
        return;
    }
    client->process();
    epoll_event ev;
    ev.data.fd=client->GetFd();
    ev.events=EPOLLOUT|EPOLLET;
    epoll_ctl(pool_ptr->m_epoll,EPOLL_CTL_MOD,client->GetFd(),&ev);
}
void processpool::OnWrite(HttpConn* client)
{
    assert(client);
    int ret=-1;
    int writeErrno=0;
    ret=client->write(&writeErrno);
    if(client->ToWriteByte()==0)
    {
        if(client->IsKeepAlive())
        {
            LOG_INFO("child process %d write completely but keep-Alive",pool_ptr->m_idx);
            client->reset();
            epoll_event ev;
            ev.data.fd=client->GetFd();
            ev.events=EPOLLIN|EPOLLET;
            epoll_ctl(pool_ptr->m_epoll,EPOLL_CTL_MOD,client->GetFd(),&ev);
            return ;
        }
        LOG_INFO("child process bug1");
    }
    else if(ret<0)
    {
        if(writeErrno==EAGAIN)
        {
            LOG_INFO("child process %d not write completely",pool_ptr->m_idx);
            epoll_event ev;
            ev.data.fd=client->GetFd();
            ev.events=EPOLLOUT|EPOLLET;
            epoll_ctl(pool_ptr->m_epoll,EPOLL_CTL_MOD,client->GetFd(),&ev);
            return;
        }
        LOG_INFO("child process bug2");
    }
    LOG_INFO("child process %d close the client %d",pool_ptr->m_idx,client->GetFd());
    CloseConn(client);
}

void processpool::CloseConn(HttpConn* client)
{
    assert(client);
    epoll_event ev;
    epoll_ctl(pool_ptr->m_epoll,EPOLL_CTL_DEL,client->GetFd(),&ev);
    client->Close();
}