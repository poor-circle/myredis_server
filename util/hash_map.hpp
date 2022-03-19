#pragma once
#include "sparsepp/spp.h"
namespace myredis
{
	template<class T1,class T2>
	using hash_map = spp::sparse_hash_map<T1,T2>;
	//使用这种高效且节省内存的hash_map(底层使用开放寻址法解决hash冲突）
	//来代替标准库的unordered_map（底层使用拉链法解决hash冲突）

}
