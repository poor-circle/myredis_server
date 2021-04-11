#include "stdafx.h"
#include "BaseSession.h"
//ÍøÂç¿ò¼Ü£¬¸ºÔð¼àÌý¶Ë¿Ú
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