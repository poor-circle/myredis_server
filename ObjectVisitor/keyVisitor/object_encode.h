#pragma once
#include "../../stdafx.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    inline string object_encode(int64_t object) 
    { 
        return "int64"; 
    }
    inline string object_encode(double object)
    { 
        return "double"; 
    }
    inline string object_encode(std::unique_ptr<string>& object)
    {
        auto dis = (char*)object->data() - (char*)object.get();
        if (dis >= 0 && dis < sizeof(string))
            return "embstr";
        else
            return "raw";
    }
    inline string object_encode(std::unique_ptr<hash_set<string>>& object)
    {
        return "hash_set";
    }
    inline string object_encode(std::unique_ptr<hash_map<string, string>>& object)
    {
        return "hash_map";
    }
    inline string object_encode(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return "rbtree";
    }
    inline string object_encode(std::unique_ptr<deque<string>>& object)
    {
        return "deque";
    }
}
