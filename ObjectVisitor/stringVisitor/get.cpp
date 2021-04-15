#include "stdafx.h"
#include "get.h"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
//visitor�������̬��Ϊ���������麯����
//get������ֻ�������ַ������󣬲��������������͵Ķ���
namespace myredis::visitor
{
    std::pair<code::code,string&>  get(int64_t object)
    {
        static string temp;
        temp = boost::lexical_cast<string>(object);
        return myredis_succeed(temp);
    }
    std::pair<code::code, string&> get(double object)
    {
        static string temp;
        temp.clear();
        fmt::format_to(std::back_inserter(temp),FMT_COMPILE("{}"), object);
        return myredis_succeed(temp);
    }
    std::pair<code::code, string&> get(std::unique_ptr<string>& object)
    {
        return myredis_succeed(*object);
    }
    std::pair<code::code, string&> get(std::unique_ptr<hash_set<string>>& object)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::code, string&> get(std::unique_ptr<hash_map<string, string>>& object)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::code, string&> get(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::code, string&> get(std::unique_ptr<deque<string>>& object)
    {
        return myredis_failed(object_type_error);
    }
}
