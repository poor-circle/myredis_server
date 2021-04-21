#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    std::pair<code::status, string&> lrange(int64_t& value, int64_t start,int64_t end);
    std::pair<code::status, string&> lrange(double& value, int64_t start, int64_t end);
    std::pair<code::status, string&> lrange(std::unique_ptr<string>& value, int64_t start, int64_t end);
    std::pair<code::status, string&> lrange(std::unique_ptr<hash_set<string>>& value, int64_t start, int64_t end);
    std::pair<code::status, string&> lrange(std::unique_ptr<hash_map<string, string>>& value, int64_t start, int64_t end);
    std::pair<code::status, string&> lrange(std::unique_ptr<key_ordered_map<double, string>>& value, int64_t start, int64_t end);
    std::pair<code::status, string&> lrange(std::unique_ptr<deque<string>>& value, int64_t start, int64_t end);
}
