#pragma once
#include "../stdafx.h"
//每一个visitor都要实现所有类型的操作
namespace myredis
{
    string object_encode(int64_t object)
    {
        return "int64";
    }
    string object_encode(double object)
    {
        return "double";
    }
    string object_encode(std::unique_ptr<string>& object)
    {
        return "string";
    }
    string object_encode(std::unique_ptr<hash_set<string>>& object)
    {
        return "hash_set";
    }
    string object_encode(std::unique_ptr<hash_map<string, string>>& object)
    {
        return "hash_map";
    }
    string object_encode(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return "key_ordered_map";
    }
    string object_encode(std::unique_ptr<deque<string>>& object)
    {
        return "deque";
    }
}
