#include "stdafx.h"
#include "get.h"
//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    //get函数：只适用于字符串对象，不能用于其他类型的对象
    std::pair<error::code,string&>  get(int64_t object)
    {
        static string temp;
        temp = boost::lexical_cast<string>(object);
        return myredis_succeed(temp);
    }
    std::pair<error::code, string&> get(double object)
    {
        static string temp;
        temp.clear();
        fmt::format_to(std::back_inserter(temp),FMT_COMPILE("{}"), object);
        return myredis_succeed(temp);
    }
    std::pair<error::code, string&> get(std::unique_ptr<string>& object)
    {
        return myredis_succeed(*object);
    }
    std::pair<error::code, string&> get(std::unique_ptr<hash_set<string>>& object)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<error::code, string&> get(std::unique_ptr<hash_map<string, string>>& object)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<error::code, string&> get(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<error::code, string&> get(std::unique_ptr<deque<string>>& object)
    {
        return myredis_failed(object_type_error);
    }
}
