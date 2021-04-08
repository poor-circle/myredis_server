#pragma once
#include "stdafx.h"
namespace myredis
{
	class ChatRoomSession;
	class ChatRoom
	{
		std::deque<std::shared_ptr<std::vector<char>>> messages;
		std::set<ChatRoomSession*> sessions;
	public:
		void PushMessage(std::shared_ptr<std::vector<char>> message, ChatRoomSession* from);
		void AddSession(ChatRoomSession& session);
		void RemoveSession(ChatRoomSession& session);
		static constexpr size_t queue_max_size = 64;
	};
}


