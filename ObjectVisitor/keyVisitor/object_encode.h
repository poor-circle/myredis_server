#pragma once
#include "../../stdafx.h"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
namespace myredis::visitor
{
    string object_encode(int64_t object);
    string object_encode(double object);
    string object_encode(std::unique_ptr<string>& object);
    string object_encode(std::unique_ptr<hash_set<string>>& object);
    string object_encode(std::unique_ptr<hash_map<string, string>>& object);
    string object_encode(std::unique_ptr<key_ordered_map<double, string>>& object);
    string object_encode(std::unique_ptr<deque<string>>& object);
}
