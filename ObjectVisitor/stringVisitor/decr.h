#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//每一个visitor都要实现所有类型的操作
//visitor层函数，对value对象执行具体的操作
//value对象可能有多种编码和类型，对于不同的编码需要不同的实现方式，这就是多态性：
namespace myredis::visitor
{
    //incr函数：只适用于int64_t对象，不能用于其他类型的对象
    std::pair<code::status, int64_t> decr(int64_t& object);
    std::pair<code::status, int64_t> decr(double& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<string>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<hash_set<string>>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<hash_map<string, string>>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<key_ordered_map<double, string>>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<deque<string>>& object);

    //incrby函数：只适用于int64_t对象，不能用于其他类型的对象
    std::pair<code::status, int64_t> decrby(int64_t& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(double& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<string>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<hash_set<string>>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<hash_map<string, string>>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<key_ordered_map<double, string>>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<deque<string>>& object, int64_t decrment);
}