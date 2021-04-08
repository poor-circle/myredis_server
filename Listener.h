#include "stdafx.h"
#include "BaseSession.h"
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