#pragma once
#include "../stdafx.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
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
        auto dis = (char*)object->data() - (char*)object.get();
        if (dis >= 0 && dis < sizeof(string))
            return "embstr";
        else
            return "raw";
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
