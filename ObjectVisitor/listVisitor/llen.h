#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    std::pair<code::status, int64_t> llen(int64_t& value);
    std::pair<code::status, int64_t> llen(double& value);
    std::pair<code::status, int64_t> llen(std::unique_ptr<string>& value);
    std::pair<code::status, int64_t> llen(std::unique_ptr<hash_set<string>>& value);
    std::pair<code::status, int64_t> llen(std::unique_ptr<hash_map<string, string>>& value);
    std::pair<code::status, int64_t> llen(std::unique_ptr<key_ordered_map<double, string>>& value);
    std::pair<code::status, int64_t> llen(std::unique_ptr<deque<string>>& value);
}
