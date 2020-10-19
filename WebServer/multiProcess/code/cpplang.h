#ifndef CPPLANG__H
#define CPPLANG__H

#include<iostream>
#include <stdlib.h>
#include<vector>
#include<string>
#include<cassert>
#include<list>
#include<semaphore.h>
#include<thread>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<future>
#include<algorithm>
#include<exception>
#include<queue>
#include<set>
#include<map>

#include<ctime>
#include<sys/time.h>
#include<chrono>

#include<sys/socket.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<unistd.h>  // write
#include<sys/uio.h> // readv/writev
#include<arpa/inet.h> // sockaddr_in
#include<fcntl.h>       // open
#include<sys/stat.h>    // stat
#include<sys/mman.h>
#include<errno.h>     
#include<unordered_map>
#include<unordered_set>
#include<regex>

#define BEGIN_NAMESPACE(x) namespace x{
#define END_NAMESPACE(x) }
#define USING_NAMESPACE(x) using naneapce x

#if __cpp_static_assert>=201411
#   define STATIC_ASSERT(x) static_assert(x)
#else
#   define STATIC_ASSERT(x) static_assert(x,#x)
#endif


#endif
