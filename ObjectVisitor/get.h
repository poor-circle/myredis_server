#pragma once
#include "../stdafx.h"
#include "../error.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    //get函数：只适用于字符串对象，不能用于其他类型的对象
    std::pair<error::code, string&> get(int64_t object);
    std::pair<error::code, string&> get(double object);
    std::pair<error::code, string&> get(std::unique_ptr<string>& object);
    std::pair<error::code, string&> get(std::unique_ptr<hash_set<string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<hash_map<string, string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<key_ordered_map<double, string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<deque<string>>& object);
}
