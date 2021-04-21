#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    std::pair<code::status, int64_t> lpush(int64_t& value,string& pushContent);
    std::pair<code::status, int64_t> lpush(double& value, string& pushContent);
    std::pair<code::status, int64_t> lpush(std::unique_ptr<string>& value, string& pushContent);
    std::pair<code::status, int64_t> lpush(std::unique_ptr<hash_set<string>>& value, string& pushContent);
    std::pair<code::status, int64_t> lpush(std::unique_ptr<hash_map<string, string>>& value, string& pushContent);
    std::pair<code::status, int64_t> lpush(std::unique_ptr<key_ordered_map<double, string>>& value, string& pushContent);
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, string& pushContent);
}
