#include "stdafx.h"
#include "threadPool.h"

asio::thread_pool& myredis::getThreadPool()//获取执行非核心操作的线程池
{
    static asio::thread_pool pool;
    return pool;
}
