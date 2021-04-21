#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lpush ֻ������list����
    template<typename T>
    std::pair<code::status, int64_t> lpush(T& value, string& pushContent) 
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, string& pushContent)
    {
        // ͷ������
        value->push_front(pushContent);
        // ���ظ���
        return myredis_succeed(value->size());
    }
}
