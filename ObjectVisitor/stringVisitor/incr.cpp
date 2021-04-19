#include "stdafx.h"
#include "incr.h"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
//visitor�������̬��Ϊ���������麯����
//incr������ֻ�������������󣬲��������������͵Ķ���

namespace myredis::visitor
{
    std::pair<code::status, int64_t>  incr(int64_t& object)
    {
        if (object == INT64_MAX) {
            return {code::status::value_overflow,object};
        }
        object += 1;
        return myredis_succeed(object);
    }
    std::pair<code::status, int64_t> incr(double& object)
    {
        return { code::status::object_type_error,(int64_t)0 };
    }
    std::pair<code::status, int64_t> incr(std::unique_ptr<string>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incr(std::unique_ptr<hash_set<string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incr(std::unique_ptr<hash_map<string, string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incr(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incr(std::unique_ptr<deque<string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
}
