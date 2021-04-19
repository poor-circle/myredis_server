#include "stdafx.h"
#include "changeCodetoString.h"
namespace myredis::visitor
{
    std::pair<code::status, string&> changeCodetoString(int64_t& value, object& obj)
    {
        string s;
        fmt::format_to(std::back_inserter(s), FMT_COMPILE("{}"), value);
        //将对象转换为string
        obj = std::make_unique<string>(std::move(s));
        return myredis_succeed(*std::get<std::unique_ptr<string>>(obj));
    }

    std::pair<code::status, string&> changeCodetoString(double& value, object& obj)
    {
        string s;
        fmt::format_to(std::back_inserter(s), FMT_COMPILE("{}"), value);
        //将对象转换为string
        obj = std::make_unique<string>(std::move(s));
        return myredis_succeed(*std::get<std::unique_ptr<string>>(obj));
    }
    std::pair<code::status, string&> changeCodetoString(std::unique_ptr<string>& value, object& obj)
    {
        return myredis_succeed(*value);
    }

    std::pair<code::status, string&> changeCodetoString(std::unique_ptr<hash_set<string>>& value, object& obj)
    {
        return myredis_failed(object_type_error);
    }

    std::pair<code::status, string&> changeCodetoString(std::unique_ptr<hash_map<string, string>>& value, object& obj)
    {
        return myredis_failed(object_type_error);
    }

    std::pair<code::status, string&> changeCodetoString(std::unique_ptr<key_ordered_map<double, string>>& value, object& obj)
    {
        return myredis_failed(object_type_error);
    }

    std::pair<code::status, string&> changeCodetoString(std::unique_ptr<deque<string>>& value, object& obj)
    {
        return myredis_failed(object_type_error);
    }
}

