#include "stdafx.h"
#include "Listener.h"
namespace myredis
{
    Listener::Listener(asio::io_context& ioc, uint16_t port) :ioc(ioc), port(port) {}
    asio::awaitable<void> Listener::Run()
    {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::acceptor acceptor(executor, { asio::ip::tcp::v4(), port });
        for (;;)
        {
            asio::co_spawn(executor, BaseSession::Run(std::make_unique<BaseSession>
            (
                ioc,                                                //io_context
                co_await acceptor.async_accept(asio::use_awaitable) //socket
            )), asio::detached);
        }
    }
}