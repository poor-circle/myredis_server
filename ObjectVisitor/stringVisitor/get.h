#pragma once
#include "../../stdafx.h"
#include "../../code.h"
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
