#pragma once
#include "stdafx.h"
#include "object.hpp"
//网络框架库，代表一个基本的连接会话
namespace myredis
{
    class BaseSession
    {
    public:
        BaseSession(asio::io_context& ioc, asio::ip::tcp::socket socket);
        static asio::awaitable<void> run(std::unique_ptr<BaseSession> self);
        void setDataBaseID(int64_t ID)  noexcept;
        objectMap& getObjectMap() noexcept;
        void setClosed() noexcept;
        bool isLogined() noexcept;
        void setLogined(bool logined) noexcept;
        asio::awaitable<std::vector<string>> block();
        std::vector<string> args_for_block;
        bool isBlocked() noexcept;
        void setBlocked(std::vector<string>&& new_args);
        BaseSession(const BaseSession&) = delete;
        BaseSession(BaseSession&&) = delete;
        BaseSession& operator =(BaseSession&&) = delete;
        BaseSession& operator =(const BaseSession&) = delete;
        static hash_set<void*>& getSessionSet();
        ~BaseSession();
        void wake_up();
    private:
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        asio::steady_timer clock;
        int64_t dataBaseID;
        bool closed;
        bool logined;
        bool blocked;
    };
}