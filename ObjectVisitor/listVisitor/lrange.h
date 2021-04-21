#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, string&> lrange(T& value, int64_t start, int64_t end)
    {

    }
    template<> inline
    std::pair<code::status, string&> lrange(std::unique_ptr<deque<string>>& value, int64_t start, int64_t end)
    {

    }
}
