#pragma once
#include "stdafx.h"
#include "Session.h"
namespace myredis
{
#include"namespace.h"
    Session::Session(asio::io_context& ioc, asio::ip::tcp::socket socket) :ioc(ioc), socket(std::move(socket)) {}
    std::unique_ptr<Session> Session::CreateSession(asio::io_context& ioc, asio::ip::tcp::socket socket, uint32_t sessionType)
    {
        auto iter = GetSessionTable().find(sessionType);
        if (iter == GetSessionTable().end())
        {
            return nullptr;
        }
        else
        {
            return iter->second(ioc, std::move(socket));
        }
    }
    std::unordered_map<uint32_t, Session::RegisterFunction>& Session::GetSessionTable()
    {
        static std::unordered_map<uint32_t, Session::RegisterFunction> SessionTable{};
        return SessionTable;
    }
}
