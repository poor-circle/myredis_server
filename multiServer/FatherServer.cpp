#include "stdafx.h"
#include "fatherServer.h"
namespace myredis
{
    using namespace std::chrono;

    asio::awaitable<void> FatherServer::run(std::string_view address,uint16_t port)
    {
        /*asio::ip::tcp::resolver res(ioc);
        auto ep = asio::ip::tcp::endpoint(asio::ip::make_address_v4(address), port);
        auto g=co_await res.async_resolve(ep, asio::use_awaitable);
        co_await asio::async_connect(soc, ep, asio::use_awaitable);
        uint64_t sz;
        co_await asio::async_read(soc, asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
        string buf;
        buf.resize(sz);
        co_await asio::async_read(soc, asio::buffer(buf.data(), sz), asio::use_awaitable);
        */
        co_return;
        
    }
    FatherServer::FatherServer(asio::io_context& ioc):ioc(ioc),soc(ioc){}
}

