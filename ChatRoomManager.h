#pragma once
#include"stdafx.h"
#include"ChatRoom.h"
namespace myredis
{
	class ChatRoomSession;
	class ChatRoomManager
	{
		std::unordered_map<uint64_t, ChatRoom> manager;
	public:
		static ChatRoomManager& getManager();

		ChatRoom* openChatRoom(uint64_t ChatRoomID, ChatRoomSession& session);

		void closeChatRoom(uint64_t ChatRoomID, ChatRoomSession& session);
		
	};
}
