#pragma once
#include <utility>
#include <map>
namespace myredis
{
	template<typename T1,typename T2>
	using key_ordered_map = std::multimap<T1, T2>;
	//TODO:std::multimap 不支持rank查询
}