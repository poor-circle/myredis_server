#pragma once
#include "../stdafx.h"
#include "../code.h"
#include "../object.hpp"

//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    // 获取对象的序列化字符串，用于RDB持久化
    
    // 格式： 类型 value----string
    //格式：
    template<typename T>
    string serialize(object& obj)
    {
        return "类型错误";
    }
    template<> inline
    string serialize<int64_t>(object& obj)
    {
        string temp;
        temp.assign(9, '\0');
        temp[0] = (char)obj.index();
        memcpy(temp.data() + 1, &std::get<int64_t>(obj), sizeof(int64_t));
        return temp;
    }

    template<> inline
    string serialize<double>(object& obj)
    {
        string temp;
        temp.assign(9, '\0');
        temp[0] = (char)obj.index();
        memcpy(temp.data() + 1, &std::get<double>(obj), sizeof(double));
        return temp;
    }

    template<> inline
        string serialize<std::unique_ptr<string>>(object& obj)
    {
        string temp;
        temp.push_back((char)obj.index());
        temp += *std::get<std::unique_ptr<string>>(obj);
        return temp;
    }
    // set
    // 格式： 类型（1字节） 个数（8字节） 数据（按照C语言\0的格式存) --- list、set
    template<> inline
        string serialize<std::unique_ptr<hash_set<string>>>(object& obj)
    {
        
        string temp;
        temp.assign(9, '\0');
        temp[0] = (char)obj.index();
        hash_set<string>& value = *std::get<std::unique_ptr<hash_set<string>>>(obj);
        auto s = value.size();
        memcpy(temp.data() + 1, &s, sizeof(s));
        for (auto& i : value) {
            temp += i;
        }
        return temp;
    }

    // list
    // 格式： 类型（1字节） 个数（8字节） 数据（按照C语言\0的格式存) --- list、set
    template<> inline
        string serialize<std::unique_ptr<deque<string>>>(object& obj)
    {
        string temp;
        temp.assign(9, '\0');
        temp[0] = (char)obj.index();
        deque<string>& value = *std::get<std::unique_ptr<deque<string>>>(obj);
        auto s = value.size();
        memcpy(temp.data() + 1, &s, sizeof(s));
        for (auto& i : value) {
            temp += i;
        }
        return temp;
    }

    string KVSerialize(string& key, object& obj) {
        auto ret = std::visit([&obj](auto& e) {return serialize<decltype(e)>(obj); }, obj);
        return key + ret;
    }
}
