#pragma once
#include "../stdafx.h"
#include "../code.h"
#include "../object.hpp"

//每一个visitor都要实现所有类型的操作
namespace myredis::visitor
{
    // 获取对象的序列化字符串，用于RDB持久化

    inline void appendObjectType(const object& obj, string& ret)
    {
        ret += (unsigned char)obj.index();
    }

    template<typename T>
    inline void appendBinary(const T*const sz, string& ret)
    {
        auto pos = ret.length();
        ret.append(sizeof(*sz), '\0');
        memcpy(ret.data() + pos, sz, sizeof(*sz));
    }
    //格式： 类型 value----string
    //格式：
    template<typename T>
        void serialize(const object& obj, string& ret)
    {
        throw std::exception("未实现对应的序列化函数！");
    }
    template<> inline
        void serialize<const int64_t&>(const object& obj, string& ret)
    {
        appendObjectType(obj, ret);
        appendBinary(&std::get<int64_t>(obj), ret);
    }

    template<> inline
        void serialize<const double&>(const object& obj, string& ret)
    {
        appendObjectType(obj, ret);
        appendBinary(&std::get<double>(obj), ret);
    }

    template<> inline
        void serialize<const std::unique_ptr<string>&>(const object& obj, string& ret)
    {
        appendObjectType(obj, ret);
        auto& str = *std::get<std::unique_ptr<string>>(obj);
        auto strlength = str.length();
        appendBinary(&strlength, ret);
        ret += *std::get<std::unique_ptr<string>>(obj);
    }
    // set
    // 格式： 类型（1字节） 个数（8字节） 数据（按照C语言\0的格式存) --- list、set
    template<> inline
        void serialize<const std::unique_ptr<hash_set<string>>&>(const object& obj, string& ret)
    {
        appendObjectType(obj, ret);
        hash_set<string>& value = *std::get<std::unique_ptr<hash_set<string>>>(obj);
        auto s = value.size();
        appendBinary(&s, ret);
        for (auto& i : value) {
            auto s = i.length();
            appendBinary(&s, ret);
            ret += i;
        }
    }

    // list
    // 格式： 类型（1字节） 个数（8字节） 数据（按照C语言\0的格式存) --- list、set
    template<> inline
        void serialize<const std::unique_ptr<deque<string>>&>(const object& obj, string& ret)
    {
        appendObjectType(obj, ret);
        deque<string>& value = *std::get<std::unique_ptr<deque<string>>>(obj);
        auto s = value.size();
        appendBinary(&s, ret);
        for (auto& i : value) {
            auto s = i.length();
            appendBinary(&s, ret);
            ret += i;
        }
    }

    inline void KVSerialize(const string& key, const object& obj, string& ret) {
        auto s = key.length();
        appendBinary(&s, ret);
        ret += key;
        std::visit([&](auto& e) { serialize<decltype(e)>(obj,ret); }, obj);
    }

    inline void DBSerialize(objectMap& db,string&ret)
    {
        auto sizetemp = db.size();
        appendBinary(&sizetemp, ret);
        for (auto iter = db.keyBegin(); iter != db.keyEnd(); ++iter)
        {
            KVSerialize(iter->getStr(),db.find(iter),ret);
        }
    }

    inline string MultiDBSerialize()
    {
        string ret;
        ret.assign(sizeof(size_t), '\0');
        size_t temp = data_base_count;
        memcpy(ret.data(), &temp, sizeof(temp));
        for (size_t i = 0; i < data_base_count; ++i)
        {
            auto& db = objectMap::getObjectMap(i);
            DBSerialize(db,ret);
        }
        return ret;
    }
}
