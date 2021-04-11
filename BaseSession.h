#pragma once
#include "stdafx.h"
//网络框架库，代表一个基本的连接会话
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