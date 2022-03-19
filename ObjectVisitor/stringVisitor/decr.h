#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//每一个visitor都要实现所有类型的操作
//visitor层函数，对value对象执行具体的操作
//value对象可能有多种编码和类型，对于不同的编码需要不同的实现方式，这就是多态性：
namespace myredis::visitor
{
    // decr:只适用于int64对象
    template<typename T>
    std::pair<code::status, int64_t> decr(T& object)
    {
        return { code::status::object_type_error,0 };
    }

    template<> inline
    std::pair<code::status, int64_t> decr(int64_t& object)
    {
        if (object == INT64_MIN) {
            return { code::status::value_overflow,object };
        }
        object -= 1;
        return myredis_succeed(object);
    }

    // decrby:只适用于int64对象
    template<typename T>
    std::pair<code::status, int64_t> decrby(T& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline 
    std::pair<code::status, int64_t> decrby(int64_t& object, int64_t decrement)
    {

        if ((object > INT64_MAX + decrement && decrement < 0) ||
            (object < INT64_MIN + decrement && decrement > 0))
        {
            return { code::status::value_overflow,object };
        }
        object -= decrement;
        return myredis_succeed(object);
    }
}