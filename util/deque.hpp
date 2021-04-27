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
	//deque���͵�����ǿ��˫������,�ռ�ռ�õ���˫������֧�ְ�index���ٲ�ѯ
	//С���⣺��֪����λ��ʱ���Ӷ�����list
}
