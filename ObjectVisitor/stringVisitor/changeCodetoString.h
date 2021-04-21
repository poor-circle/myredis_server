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
    //get������ֻ�������ַ������󣬲��������������͵Ķ���
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
        //������ת��Ϊstring
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
