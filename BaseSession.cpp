#include "stdafx.h"
#include "BaseSession.h"
#include "Session.h"

namespace myredis
{

#include "namespace.h"

    BaseSession::BaseSession(asio::io_context& ioc, tcp::socket socket) :ioc(ioc), socket(move(socket)) {}

    awaitable<void> BaseSession::Run(unique_ptr<BaseSession> self)
    {

        try
        {
            //auto sessionType = co_await self->getSessionType();
            auto session = Session::CreateSession(self->ioc, std::move(self->socket), 0);
            if (session != nullptr)
                co_await session->Run();
            else
            {
              //  fmt::print("Error: Unknown Session Type({})!\n", sessionType);
            }
        }
        catch (exception e) {};
        co_return;
    }

    awaitable<uint32_t> BaseSession::getSessionType()
    {
        uint32_t ret;
        co_await asio::async_read(socket, asio::buffer(&ret, sizeof(ret)), use_awaitable);
        co_return ret;
    }

}