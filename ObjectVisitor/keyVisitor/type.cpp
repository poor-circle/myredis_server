#pragma once
#include "stdafx.h"
#include "type.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    string type(int64_t object)
    {
        return "string";
    }
    string type(double object)
    {
        return "string";
    }
    string type(std::unique_ptr<string>& object)
    {
        return "string";
    }
    string type(std::unique_ptr<hash_set<string>>& object)
    {
        return "set";
    }
    string type(std::unique_ptr<hash_map<string, string>>& object)
    {
        return "hash";
    }
    string type(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return "zset";
    }
    string type(std::unique_ptr<deque<string>>& object)
    {
        return "list";
    }
}
