#pragma once
#include "sparsepp/spp.h"
namespace myredis
{
	template<class T>
	using hash_set = spp::sparse_hash_set<T>;
	//使用这种高效且节省内存的hash_set(底层使用开放寻址法解决hash冲突）
	//来代替标准库的unordered_set（底层使用拉链法解决hash冲突）
}