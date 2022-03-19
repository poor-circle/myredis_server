#include "stdafx.h"
#include "BaseSession.h"
//网络框架，负责监听端口
namespace myredis
{
    class Listener
    {
    public:
        asio::io_context& ioc;
        const uint16_t port;
        Listener(asio::io_context& ioc, uint16_t port);
        asio::awaitable<void> Run();
        
    };
}