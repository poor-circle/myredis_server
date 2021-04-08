#pragma once
#include "stdafx.h"
namespace myredis
{
    class BaseSession
    {
        friend class Session;
    public:
        BaseSession(asio::io_context& ioc, asio::ip::tcp::socket socket);
        static asio::awaitable<void> Run(std::unique_ptr<BaseSession> self);
    private:
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        asio::awaitable<uint32_t> getSessionType();
    };
}