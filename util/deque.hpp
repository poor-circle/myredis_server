#pragma once
#include <deque>
namespace myredis
{
#define fod(name)\
template<typename... Args>\
decltype(auto) name(Args&&... args)\
{\
	return que.##name(std::forward<Args>(args)...);\
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
		std::deque<T> que;
		std::deque<asio::steady_timer*> clocks;

		fod(insert)
		fod(begin)
		fod(end)
		fod_void(clear)
		fod(empty)
		fod(emplace_back)
		fod(emplace_front)
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
	};
	//deque类型的性能强于双向链表,空间占用低于双向链表，支持按index快速查询
	//小问题：已知插入位置时复杂度劣于list
}
