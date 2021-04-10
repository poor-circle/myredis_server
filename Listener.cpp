#include "stdafx.h"
#include "Listener.h"
namespace myredis
{
    Listener::Listener(asio::io_context& ioc, uint16_t port) :ioc(ioc), port(port) {}
    asio::awaitable<void> Listener::Run()
    {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::acceptor acceptor(executor, { asio::ip::tcp::v4(), port });
        fmt::print
        (
            "*{0:*^{2}}*\n"
            "*{0: ^{2}}*\n"
            "*{1: ^{2}}*\n"
            "*{0: ^{2}}*\n"
            "*{0:*^{2}}*\n\n",
            "", "myredis start listening at port:"+std::to_string(myredis::defaultPort), 100
        );
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