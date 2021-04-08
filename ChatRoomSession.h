#pragma once
#include"stdafx.h"
#include"Session.h"
namespace myredis
{
    class ChatRoomManager;
    class ChatRoom;
    class ChatRoomSession :Session
    {
    private:
        static uint32_t sessionTypeCode;
        ChatRoom* chatRoom;
        std::queue<std::shared_ptr<std::vector<char>>> messages;
        asio::awaitable<uint64_t> getRoomID();
        asio::awaitable<std::shared_ptr<std::vector<char>>> Receive();
        asio::awaitable<void> Send();

        bool sendFlag = false;
    public:

        ChatRoomSession(asio::io_context& ioc, asio::ip::tcp::socket socket);

        virtual asio::awaitable<void> Run();

        void TrySend();

        void PushMessage(std::shared_ptr<std::vector<char>> message);
    };
}