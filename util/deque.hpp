#pragma once
#include <deque>
#include "deque_helper.h"
namespace myredis
{
#define fod(name)\
template<typename... Args>\
decltype(auto) name(Args&&... args)\
{\
	return que.##name(std::forward<Args>(args)...);\
}\

#define fod_check(name)\
template<typename... Args>\
decltype(auto) name(Args&&... args)\
{\
	decltype(auto) ret=que.##name(std::forward<Args>(args)...);\
	try_to_wake_up();\
	return ret;\
}\

#define fod_void(name)\
template<typename... Args>\
void name(Args&&... args)\
{\
	return que.##name(std::forward<Args>(args)...);\
}\


#define fod_static(name)\
template<typename... Args>\
static decltype(auto) name(Args&&... args)\
{\
	return std::deque<T>::##name(std::forward<Args>(args)...);\
}\

	template<class T>
	struct deque
	{
	private:
		std::deque<T> que;
		std::deque<void*> clocks;
	public:
		fod_check(insert)
		fod(begin)
		fod(end)
		fod_void(clear)
		fod(empty)
		fod_check(emplace_back)
		fod_check(emplace_front)
		fod(front)
		fod_void(pop_back)
		fod(size)
		fod_void(pop_front)
		fod(erase)
		fod(rbegin)
		fod(rend)
		fod(back)
		fod_static(reverse_iterator)

		T& operator[](size_t _Pos) noexcept
		{
			return que[_Pos];
		}
		const T& operator[](size_t _Pos) const noexcept
		{
			return que[_Pos];
		}

		void try_to_wake_up(void)
		{
			wake_up_session(clocks);
		}

		void watched_session(void *baseSession_Ptr)
		{
			clocks.emplace_back(baseSession_Ptr);
		}
	};
	//deque类型的性能强于双向链表,空间占用低于双向链表，支持按index快速查询
	//小问题：已知插入位置时复杂度劣于list
}
