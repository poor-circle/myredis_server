#pragma once
#include <deque>
namespace myredis
{

	template<class T>
	using deque= std::deque<T>;
	//deque类型的性能强于双向链表,空间占用低于双向链表，支持按index快速查询
	//小问题：已知插入位置时复杂度劣于list
}
