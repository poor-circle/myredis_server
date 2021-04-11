#pragma once
#include "stdafx.h"
//多态连接，可以派生出不同的连接类型（在本项目中，目前只有一种连接）
namespace myredis
{
    class Session
    {
    protected:
        using RegisterFunction = std::unique_ptr<Session>(*)(asio::io_context& ioc, asio::ip::tcp::socket socket);
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        Session(asio::io_context& ioc, asio::ip::tcp::socket socket);
        static std::unordered_map<uint32_t, Session::RegisterFunction>& GetSessionTable();
        template<typename T>
        static auto factory(asio::io_context& ioc, asio::ip::tcp::socket socket)
        {
            return std::unique_ptr<Session>((Session*)new T(ioc, std::move(socket)));
        }
        template<typename T, uint32_t code>
        static uint32_t regist()
        {
            auto iter = GetSessionTable().find(code);
            if (iter == GetSessionTable().end())
            {
                GetSessionTable().insert({ code , factory<T> });
            }
            else
            {
                fmt::print("Repeat Session Type!\n");
                throw std::exception("Repeat Session Type!");
            }
            return code;
        }
    public:

        virtual asio::awaitable<void> Run() = 0;

        static std::unique_ptr<Session> CreateSession(asio::io_context& ioc, asio::ip::tcp::socket socket, uint32_t sessionType);

       
    };
}
