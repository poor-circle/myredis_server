#include "stdafx.h"
#include "decr.h"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
//visitor�������̬��Ϊ���������麯����
//decr������ֻ�������������󣬲��������������͵Ķ���

namespace myredis::visitor
{
    // decr
    std::pair<code::status, int64_t>  decr(int64_t& object)
    {
        if (object == INT64_MIN) {
            return { code::status::value_overflow,object };
        }
        object -= 1;
        return myredis_succeed(object);
    }
    std::pair<code::status, int64_t> decr(double& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decr(std::unique_ptr<string>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decr(std::unique_ptr<hash_set<string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decr(std::unique_ptr<hash_map<string, string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decr(std::unique_ptr<key_ordered_map<double, string>>& object)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decr(std::unique_ptr<deque<string>>& object)
    {
        return { code::status::object_type_error,0 };
    }

    // decrby
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
    std::pair<code::status, int64_t> decrby(double& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decrby(std::unique_ptr<string>& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decrby(std::unique_ptr<hash_set<string>>& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decrby(std::unique_ptr<hash_map<string, string>>& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decrby(std::unique_ptr<key_ordered_map<double, string>>& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> decrby(std::unique_ptr<deque<string>>& object, int64_t decrement)
    {
        return { code::status::object_type_error,0 };
    }


}
