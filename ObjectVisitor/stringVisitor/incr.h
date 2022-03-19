#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//每一个visitor都要实现所有类型的操作
//visitor层函数，对value对象执行具体的操作
//value对象可能有多种编码和类型，对于不同的编码需要不同的实现方式，这就是多态性：
//例如，get函数的底层实现：
//对于int64_t和double:需要转换成字符串
//对于string类型则直接返回字符串本身即可
//对于非字符串的类型，使用get是不合法的
namespace myredis::visitor
{
    // incr:只适用于int64_t对象
    template<typename T>
    std::pair<code::status, int64_t> incr(T& object)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t>  incr(int64_t& object)
    {
        if (object == INT64_MAX) {
            return { code::status::value_overflow,object };
        }
        object += 1;
        return myredis_succeed(object);
    }

    template<typename T>
    std::pair<code::status, int64_t> incrby(T& value, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incrby(int64_t& object, int64_t increment)
    {

        if ((object > INT64_MAX - increment && increment > 0) ||
            (object < INT64_MIN - increment && increment < 0))
        {
            return { code::status::value_overflow,object };
        }
        object += increment;
        return myredis_succeed(object);
    }

    //incrbyfloat函数:只适用于double和int64_t对象
    template<typename T>
    std::pair<code::status, string&> incrbyfloat(T& value, object& obj, double increment)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
        std::pair<code::status, string&> incrbyfloat(int64_t& value, object& obj, double increment)
    {
        static string tempStr;
        obj = value + increment;
        tempStr = boost::lexical_cast<string>(std::get<double>(obj));
        return myredis_succeed(tempStr);
    }
    template<> inline
        std::pair<code::status, string&> incrbyfloat(double& value, object& obj, double increment)
    {
        value += increment;
        static string temp;
        temp = boost::lexical_cast<string>(value);
        return myredis_succeed(temp);
    }
}