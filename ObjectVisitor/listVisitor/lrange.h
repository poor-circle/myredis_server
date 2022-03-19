#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, std::vector<string> > lrange(T& value, int64_t start, int64_t end)
    {
        return { code::status::object_type_error, std::vector<string>() };
    }
    template<> inline
    std::pair<code::status, std::vector<string> > lrange(std::unique_ptr<deque<string>>& value, int64_t start, int64_t end)
    {
        if (value->empty()) {
            return { code::status::object_is_empty, std::vector<string>() };
        }
        auto list = std::vector<string>();
        const int64_t strLen = value->size();
        // 如果小于零 + strLen ，还小于0说明超过长度了，直接置0
        if (start > strLen) 
            return myredis_succeed(std::vector<string>());
        
        if (start < 0)
            start = strLen + start;
        if (start < 0)
            start = 0;
        if (end < 0)
            end = strLen + end;
        if (end < 0)
            end = 0;
        if (end > strLen)
            end = strLen - 1;
        if (end < start)
            return myredis_succeed(std::vector<string>());
        copy(value->begin() + start, value->begin() + end + 1, std::back_inserter(list));
        return myredis_succeed(list);

    }
}
