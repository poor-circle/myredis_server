#pragma once
#include "../../stdafx.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    enum class objectType
    {
        string,
        set,
        hash,
        zset,
        list
    };
    objectType type(int64_t object)
    {
        return objectType::string;
    }
    objectType type(double object)
    {
        return objectType::string;
    }
    objectType type(std::unique_ptr<string>& object)
    {
        return objectType::string;
    }
    objectType type(std::unique_ptr<hash_set<string>>& object)
    {
        return objectType::set;
    }
    objectType type(std::unique_ptr<hash_map<string, string>>& object)
    {
        return objectType::hash;
    }
    objectType type(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return objectType::zset;
    }
    objectType type(std::unique_ptr<deque<string>>& object)
    {
        return objectType::list;
    }
}
