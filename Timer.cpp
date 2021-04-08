#include"stdafx.h"
#include"Session.h"
namespace myredis
{
#include"namespace.h"
    class Timer :Session
    {
    public:
        static uint32_t sessionTypeCode;
        Timer(asio::io_context& ioc, tcp::socket socket) :Session(ioc, std::move(socket)) {};


        virtual awaitable<void> Run()
        {
            asio::steady_timer timer(ioc);
            array<char,1024> buf;
            for (int i = 0;; ++i)
            {
                timer.expires_after(1s);
                auto len=fmt::format_to(buf.data(), "time: {}s\n", i)-buf.data();
                co_await async_write(socket, asio::buffer(buf, len), use_awaitable);
                co_await timer.async_wait(use_awaitable);
            }
        }
    };
    Regist(Timer, 1);
}