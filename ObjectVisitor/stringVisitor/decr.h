#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
//visitor�㺯������value����ִ�о���Ĳ���
//value��������ж��ֱ�������ͣ����ڲ�ͬ�ı�����Ҫ��ͬ��ʵ�ַ�ʽ������Ƕ�̬�ԣ�
namespace myredis::visitor
{
    // decr:ֻ������int64����
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

    // decrby:ֻ������int64����
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