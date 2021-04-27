#pragma once
#include<deque>
namespace myredis
{
	void wake_up_session(std::deque<void*>& s);
}
