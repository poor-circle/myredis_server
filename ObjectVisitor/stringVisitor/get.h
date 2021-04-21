#pragma once
#include "../../stdafx.h"
#include "../../code.h"
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
    std::pair<code::status, string&> get(T& object)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
    std::pair<code::status, string&> get(int64_t& object)
    {
        static string temp;
        temp = boost::lexical_cast<string>(object);
        return myredis_succeed(temp);
    }
    template<> inline 
    std::pair<code::status, string&> get(double& object)
    {
        static string temp;
        temp = string();
        fmt::format_to(std::back_inserter(temp), FMT_COMPILE("{}"), object);
        return myredis_succeed(temp);
    }
    template<> inline 
    std::pair<code::status, string&> get(std::unique_ptr<string>& object)
    {
        return myredis_succeed(*object);
    }
}
