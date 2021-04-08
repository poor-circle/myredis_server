#include "stdafx.h"
#include "ChatRoom.h"
#include "ChatRoomSession.h"
namespace myredis
{
#include "namespace.h"
	void myredis::ChatRoom::PushMessage(std::shared_ptr<std::vector<char>> message,ChatRoomSession* from)
	{
		messages.push_back(message);
		if (messages.size() > ChatRoom::queue_max_size) messages.pop_front();
		for (auto& e : sessions)
		{
			if (e != from)
			{
				e->PushMessage(message);
				e->TrySend();
			}
		}
	}
	void ChatRoom::AddSession(ChatRoomSession& session)
	{
		sessions.emplace(&session);
		for (auto& e : messages)
			session.PushMessage(e);
		session.TrySend();
	}
	void ChatRoom::RemoveSession(ChatRoomSession& session)
	{
		sessions.erase(&session);
	}
}
