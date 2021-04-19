#pragma once
#include "stdafx.h"
//�����ܿ⣬����һ�����������ӻỰ
namespace myredis
{
    class BaseSession
    {
        friend class Session;
    public:
        BaseSession(asio::io_context& ioc, asio::ip::tcp::socket socket);
        static asio::awaitable<void> Run(std::unique_ptr<BaseSession> self);
        void stop();
        int getDataBaseIndex();
        void setDataBaseIndex(int index);
    private:
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        int dataBaseIndex = 0;
    };
}