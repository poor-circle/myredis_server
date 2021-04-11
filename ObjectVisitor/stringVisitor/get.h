#pragma once
#include "../../stdafx.h"
#include "../../error.h"
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
    std::pair<error::code, string&> get(int64_t object);
    std::pair<error::code, string&> get(double object);
    std::pair<error::code, string&> get(std::unique_ptr<string>& object);
    std::pair<error::code, string&> get(std::unique_ptr<hash_set<string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<hash_map<string, string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<key_ordered_map<double, string>>& object);
    std::pair<error::code, string&> get(std::unique_ptr<deque<string>>& object);
}
