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
            "", "myredis start listening at port:"+std::to_string(port), 75
        );
        for (;;)
        {
            auto ptr = BaseSession::create(
                ioc,                                                //io_context
                co_await acceptor.async_accept(asio::use_awaitable) //socket
            );

            asio::co_spawn(ioc.get_executor(), BaseSession::run(std::move(ptr)), asio::detached);
            assert((fmt::print("listen at thread:{}\n", std::this_thread::get_id()), 1));
        }
    }
}