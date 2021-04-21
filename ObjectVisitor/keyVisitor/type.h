#pragma once
#include "../../stdafx.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    string type(int64_t object);
    string type(double object);
    string type(std::unique_ptr<string>& object);
    string type(std::unique_ptr<hash_set<string>>& object);
    string type(std::unique_ptr<hash_map<string, string>>& object);
    string type(std::unique_ptr<key_ordered_map<double, string>>& object);
    string type(std::unique_ptr<deque<string>>& object);
}
