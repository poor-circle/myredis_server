#pragma once
#include "../../stdafx.h"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
namespace myredis::visitor
{
    inline string type(int64_t object)
    {
        return "string";
    }
    inline string type(double object)
    {
        return "string";
    }
    inline string type(std::unique_ptr<string>& object)
    {
        return "string";
    }
    inline string type(std::unique_ptr<hash_set<string>>& object)
    {
        return "set";
    }
    inline string type(std::unique_ptr<hash_map<string, string>>& object)
    {
        return "hash";
    }
    inline string type(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return "zset";
    }
    inline string type(std::unique_ptr<deque<string>>& object)
    {
        return "list";
    }
}
