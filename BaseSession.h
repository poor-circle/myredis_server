#pragma once
#include "stdafx.h"
#include "object.hpp"
//网络框架库，代表一个基本的连接会话
namespace myredis
{
    class BaseSession
    {
        friend class Session;
    public:
        BaseSession(asio::io_context& ioc, asio::ip::tcp::socket socket);
        static asio::awaitable<void> run(std::unique_ptr<BaseSession> self);
        void setDataBaseID(int64_t ID)  noexcept;
        objectMap& getObjectMap() noexcept;
        void setClosed() noexcept;
        bool isLogined() noexcept;
        void setLogined(bool logined) noexcept;
    private:
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        int64_t dataBaseID;
        bool closed;
        bool logined;

    };
}