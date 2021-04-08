#include"stdafx.h"
#include"ChatRoomSession.h"
#include"ChatRoomManager.h"
#include"ChatRoom.h"
namespace myredis
{
#include"namespace.h"

    ChatRoomSession::ChatRoomSession(asio::io_context& ioc, tcp::socket socket) :
        Session(ioc, std::move(socket)),
        chatRoom{nullptr}
    {

    };

    awaitable<void> ChatRoomSession::Run()
    {
        try
        {
            chatRoom=ChatRoomManager::getManager().openChatRoom(co_await getRoomID(), *this);

            while (true)
            {
                chatRoom->PushMessage(co_await Receive(),this);
            }
        }
        catch (exception e)
        {
            fmt::print("{}\n", e.what());
        }

        socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        socket.close();
        chatRoom->RemoveSession(*this);
    }

    void ChatRoomSession::TrySend()
    {
        if (sendFlag == false && messages.empty()==false)
        {
            sendFlag = true;
            co_spawn(ioc, this->Send(), asio::detached);
        }
    }

    awaitable<uint64_t> ChatRoomSession::getRoomID()
    {
        uint64_t ID;
        co_await asio::async_read(socket, asio::buffer(&ID,sizeof(ID)), use_awaitable);
        co_return ID;
    }

    awaitable<shared_ptr<vector<char>>> ChatRoomSession::Receive()
    {
        uint64_t length;
        shared_ptr<vector<char>> message = make_shared<vector<char>>();
        co_await asio::async_read(socket, asio::buffer(&length, sizeof(length)), use_awaitable);
        message->resize(length);
        co_await asio::async_read(socket,asio::buffer(message->data(),length), use_awaitable);
        co_return message;
    }

    awaitable<void> ChatRoomSession::Send()
    {
        while (!messages.empty())
        {
            int64_t length = messages.front()->size();
            co_await asio::async_write(socket, asio::buffer(&length, sizeof(length)), use_awaitable);
            co_await asio::async_write(socket, asio::buffer(*messages.front()), use_awaitable);
            messages.pop();
        }
        sendFlag = false;
    }
    void ChatRoomSession::PushMessage(std::shared_ptr<std::vector<char>> message)
    {
        messages.emplace(message);
    }
    Regist(ChatRoomSession, 2);
}