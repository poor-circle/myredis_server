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
    std::pair<code::status, string&>  changeCodetoString(int64_t& value,object& obj);
    std::pair<code::status, string&>  changeCodetoString(double& value, object& obj);
    std::pair<code::status, string&>  changeCodetoString(std::unique_ptr<string>& value, object& obj);
    std::pair<code::status, string&>  changeCodetoString(std::unique_ptr<hash_set<string>>& value, object& obj);
    std::pair<code::status, string&>  changeCodetoString(std::unique_ptr<hash_map<string, string>>& value, object& obj);
    std::pair<code::status, string&>  changeCodetoString(std::unique_ptr<key_ordered_map<double, string>>& value, object& obj);
    std::pair<code::status, string&>  changeCodetoString(std::unique_ptr<deque<string>>& value, object& obj);
}
