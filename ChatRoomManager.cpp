#include"stdafx.h"
#include"ChatRoomManager.h"
namespace myredis
{
#include"namespace.h"
	ChatRoomManager& ChatRoomManager::getManager()
	{
		static ChatRoomManager singleItem{};
		return singleItem;
	}
	ChatRoom* ChatRoomManager::openChatRoom(uint64_t ChatRoomID,ChatRoomSession& session)
	{
		auto& room=manager.try_emplace(ChatRoomID).first->second;
		room.AddSession(session);
		return &room;
	}
	void ChatRoomManager::closeChatRoom(uint64_t ChatRoomID,ChatRoomSession& session)
	{
	}
}
