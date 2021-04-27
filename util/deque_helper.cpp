#include"stdafx.h"
#include"deque_helper.h"
#include"../BaseSession.h"
namespace myredis
{
	void wake_up_session(std::deque<void*>& s)
	{
		static auto set = BaseSession::getSessionSet();
		while (s.size())
		{
			if (set.find(s.front()) != set.end())
			{
				auto ptr = static_cast<BaseSession*>(s.front());
				ptr->wake_up();
				s.pop_front();
				break;
			}
			else
			{
				s.pop_front();
			}
		}
	}
}
