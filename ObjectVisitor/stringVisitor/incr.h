#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
//每一个visitor都要实现所有类型的操作
//visitor层函数，对value对象执行具体的操作
//value对象可能有多种编码和类型，对于不同的编码需要不同的实现方式，这就是多态性：
//例如，get函数的底层实现：
//对于int64_t和double:需要转换成字符串
//对于string类型则直接返回字符串本身即可
//对于非字符串的类型，使用get是不合法的
namespace myredis::visitor
{
    //incr函数：只适用于int64_t对象，不能用于其他类型的对象
    std::pair<code::status, int64_t> incr(int64_t& object);
    std::pair<code::status, int64_t> incr(double& object);
    std::pair<code::status, int64_t> incr(std::unique_ptr<string>& object);
    std::pair<code::status, int64_t> incr(std::unique_ptr<hash_set<string>>& object);
    std::pair<code::status, int64_t> incr(std::unique_ptr<hash_map<string, string>>& object);
    std::pair<code::status, int64_t> incr(std::unique_ptr<key_ordered_map<double, string>>& object);
    std::pair<code::status, int64_t> incr(std::unique_ptr<deque<string>>& object);

    //incrby函数：只适用于int64_t对象，不能用于其他类型的对象
    std::pair<code::status, int64_t> incrby(int64_t& object,int64_t increment);
    std::pair<code::status, int64_t> incrby(double& object, int64_t increment);
    std::pair<code::status, int64_t> incrby(std::unique_ptr<string>& object, int64_t increment);
    std::pair<code::status, int64_t> incrby(std::unique_ptr<hash_set<string>>& object, int64_t increment);
    std::pair<code::status, int64_t> incrby(std::unique_ptr<hash_map<string, string>>& object, int64_t increment);
    std::pair<code::status, int64_t> incrby(std::unique_ptr<key_ordered_map<double, string>>& object, int64_t increment);
    std::pair<code::status, int64_t> incrby(std::unique_ptr<deque<string>>& object, int64_t increment);

    //incrbyfloat函数:只适用于double对象,
    std::pair<code::status, string&> incrbyfloat(int64_t& value,object& obj,double increment);
    std::pair<code::status, string&> incrbyfloat(double& value, object& obj, double increment);
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<string>& value, object& obj, double increment);
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<hash_set<string>>& value, object& obj, double increment);
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<hash_map<string, string>>& value, object& obj, double increment);
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<key_ordered_map<double, string>>& value, object& obj, double increment);
    std::pair<code::status, string&> incrbyfloat(std::unique_ptr<deque<string>>& value, object& obj, double increment);
}