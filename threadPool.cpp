#include "stdafx.h"
#include "threadPool.h"

asio::thread_pool& myredis::getThreadPool()//��ȡִ�зǺ��Ĳ������̳߳�
{
    static asio::thread_pool pool;
    return pool;
}
