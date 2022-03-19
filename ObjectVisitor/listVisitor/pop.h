#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lpop 只适用于list对象
    template<typename T>
    std::pair<code::status, string&> lpop(T& value)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
        std::pair<code::status, string&> lpop(std::unique_ptr<deque<string>>& value)
    {
        if (value->empty()) {
            return myredis_failed(object_is_empty);
        }

        static string temp;
        temp = value->front();
        value->pop_front();
        
        return myredis_succeed(temp);
    }

    // rpop 只适用于list对象
    template<typename T>
    std::pair<code::status, string&> rpop(T& value)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
    std::pair<code::status, string&> rpop(std::unique_ptr<deque<string>>& value)
    {
        if (value->empty()) {
            return myredis_failed(object_is_empty);
        }

        static string temp;
        temp = value->back();
        value->pop_back();

        return myredis_succeed(temp);
    }
}
