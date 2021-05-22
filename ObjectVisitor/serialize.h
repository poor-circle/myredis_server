#pragma once
#include "../stdafx.h"
#include "../code.h"
#include "../object.hpp"

//ÿһ��visitor��Ҫʵ���������͵Ĳ���
namespace myredis::visitor
{
    // ��ȡ��������л��ַ���������RDB�־û�

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
    //��ʽ�� ���� value----string
    //��ʽ��
    template<typename T>
        void serialize(const object& obj, string& ret)
    {
        throw std::exception("δʵ�ֶ�Ӧ�����л�������");
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
    // ��ʽ�� ���ͣ�1�ֽڣ� ������8�ֽڣ� ���ݣ�����C����\0�ĸ�ʽ��) --- list��set
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
    // ��ʽ�� ���ͣ�1�ֽڣ� ������8�ֽڣ� ���ݣ�����C����\0�ĸ�ʽ��) --- list��set
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
