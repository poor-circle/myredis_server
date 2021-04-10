#pragma once
#include "../stdafx.h"
#include "../error.h"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
namespace myredis::visitor
{
    //get������ֻ�������ַ������󣬲��������������͵Ķ���
    std::pair<error::code, string&> get(int64_t object);
    std::pair<error::code, string&> get(double object);
    std::pair<error::code, string&> get(std::unique_ptr<string>& object);
    std::pair<error::code, string&> get(std::unique_ptr<hash_set<string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<hash_map<string, string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<key_ordered_map<double, string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<deque<string>>& object);
}
