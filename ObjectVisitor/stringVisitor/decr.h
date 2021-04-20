#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//ÿһ��visitor��Ҫʵ���������͵Ĳ���
//visitor�㺯������value����ִ�о���Ĳ���
//value��������ж��ֱ�������ͣ����ڲ�ͬ�ı�����Ҫ��ͬ��ʵ�ַ�ʽ������Ƕ�̬�ԣ�
namespace myredis::visitor
{
    //incr������ֻ������int64_t���󣬲��������������͵Ķ���
    std::pair<code::status, int64_t> decr(int64_t& object);
    std::pair<code::status, int64_t> decr(double& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<string>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<hash_set<string>>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<hash_map<string, string>>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<key_ordered_map<double, string>>& object);
    std::pair<code::status, int64_t> decr(std::unique_ptr<deque<string>>& object);

    //incrby������ֻ������int64_t���󣬲��������������͵Ķ���
    std::pair<code::status, int64_t> decrby(int64_t& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(double& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<string>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<hash_set<string>>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<hash_map<string, string>>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<key_ordered_map<double, string>>& object, int64_t decrment);
    std::pair<code::status, int64_t> decrby(std::unique_ptr<deque<string>>& object, int64_t decrment);
}