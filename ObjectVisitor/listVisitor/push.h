#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lpush 只适用于list对象
    template<typename T>
    std::pair<code::status, int64_t> lpush(T& value, string& pushContent) 
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, string& pushContent)
    {
        //  省去移动复制元素的过程
        value->emplace_front(pushContent);
        // 返回个数
        return myredis_succeed(value->size());
    }

    // rpush 只适用于list对象
    template<typename T>
    std::pair<code::status, int64_t> rpush(T& value, string& pushContent)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> rpush(std::unique_ptr<deque<string>>& value, string& pushContent)
    {
        value->emplace_back(pushContent);
        // 返回个数
        return myredis_succeed(value->size());
    }
}
