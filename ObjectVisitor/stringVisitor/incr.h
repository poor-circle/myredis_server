#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
//visitor�㺯������value����ִ�о���Ĳ���
//value��������ж��ֱ�������ͣ����ڲ�ͬ�ı�����Ҫ��ͬ��ʵ�ַ�ʽ������Ƕ�̬�ԣ�
//���磬get�����ĵײ�ʵ�֣�
//����int64_t��double:��Ҫת�����ַ���
//����string������ֱ�ӷ����ַ���������
//���ڷ��ַ��������ͣ�ʹ��get�ǲ��Ϸ���
namespace myredis::visitor
{
    // incr:ֻ������int64_t����
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

    //incrbyfloat����:ֻ������double��int64_t����
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