#pragma once
#include "../stdafx.h"
#include "../code.h"
#include "../object.hpp"

//ÿһ��visitor��Ҫʵ���������͵Ĳ���
namespace myredis::visitor
{
    // ��ȡ��������л��ַ���������RDB�־û�
    
    // ��ʽ�� ���� value----string
    //��ʽ��
    template<typename T>
    string serialize(object& obj)
    {
        return "���ʹ���";
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
    // ��ʽ�� ���ͣ�1�ֽڣ� ������8�ֽڣ� ���ݣ�����C����\0�ĸ�ʽ��) --- list��set
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
    // ��ʽ�� ���ͣ�1�ֽڣ� ������8�ֽڣ� ���ݣ�����C����\0�ĸ�ʽ��) --- list��set
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
