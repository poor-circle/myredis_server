#include "stdafx.h"
#include "incr.h"
//每一个visitor都要实现所有类型的操作
//visitor负责处理多态行为（类似于虚函数）
//incr函数：只适用于整数对象，不能用于其他类型的对象

namespace myredis::visitor
{
    // incr
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
        return { code::status::object_type_error,0 };
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

    // incrby
    std::pair<code::status, int64_t> incrby(int64_t& object, int64_t increment)
    {
        
        if((object  > INT64_MAX - increment && increment >0)||
            (object < INT64_MIN - increment && increment <0))
        {
            return { code::status::value_overflow,object };
        }
        object += increment;
        return myredis_succeed(object);
    }
    std::pair<code::status, int64_t> incrby(double& object, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incrby(std::unique_ptr<string>& object, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incrby(std::unique_ptr<hash_set<string>>& object, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incrby(std::unique_ptr<hash_map<string, string>>& object, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incrby(std::unique_ptr<key_ordered_map<double, string>>& object, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> incrby(std::unique_ptr<deque<string>>& object, int64_t increment)
    {
        return { code::status::object_type_error,0 };
    }


    //incrbyfloat函数:只适用于double和int64_t对象,
    std::pair<code::status, string&> incrbyfloat(int64_t& value, object& obj, double increment)
    {
        static string tempStr;
        obj = value + increment;
        tempStr = boost::lexical_cast<string>(std::get<double>(obj));
        return myredis_succeed(tempStr);
    }

    std::pair<code::status, string&> incrbyfloat(double& value, object& obj, double increment)
    {
        value += increment;
        static string temp;
        temp = boost::lexical_cast<string>(value);
        return myredis_succeed(temp);
    }
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<string>& value, object& obj, double increment)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<hash_set<string>>& value, object& obj, double increment)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<hash_map<string, string>>& value, object& obj, double increment)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<key_ordered_map<double, string>>& value, object& obj, double increment)
    {
        return myredis_failed(object_type_error);
    }
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<deque<string>>& value, object& obj, double increment)
    {
        return myredis_failed(object_type_error);
    }
}
