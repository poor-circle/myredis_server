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
    //get函数：只适用于字符串对象，不能用于其他类型的对象
    template<typename T>
    std::pair<code::status, string&> changeCodetoString(T& value, object& obj)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline 
    std::pair<code::status, string&> changeCodetoString(int64_t& value, object& obj)
    {
        string s;
        fmt::format_to(std::back_inserter(s), FMT_COMPILE("{}"), value);
        //将对象转换为string
        obj = std::make_unique<string>(std::move(s));
        return myredis_succeed(*std::get<std::unique_ptr<string>>(obj));
    }
    template<> inline 
    std::pair<code::status, string&> changeCodetoString(double& value, object& obj)
    {
        string s;
        fmt::format_to(std::back_inserter(s), FMT_COMPILE("{}"), value);
        
        obj = std::make_unique<string>(std::move(s));
        return myredis_succeed(*std::get<std::unique_ptr<string>>(obj));
    }
    template<> inline
    std::pair<code::status, string&> changeCodetoString(std::unique_ptr<string>& value, object& obj)
    {
        return myredis_succeed(*value);
    }
}
