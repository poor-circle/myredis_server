#pragma once
#include"stdafx.h"
namespace myredis
{
	asio::thread_pool& getThreadPool();//获取执行非核心操作的线程池
}