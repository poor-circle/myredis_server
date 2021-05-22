#pragma once
#include<cstdio>
#include"../object.hpp"
namespace myredis::visitor
{


    inline void stringUnserialize(FILE* fp, string& str)
    {
        size_t sz;
        auto cnt = fread(&sz, sizeof(sz), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
        str.resize(sz);
        cnt = fread(str.data(), sizeof(char), sz, fp);
        if (cnt != sz) 
            throw std::exception("");
    }

    inline string stringUnserialize(FILE* fp)
    {
        string str;
        stringUnserialize(fp, str);
        return str;
    }
   
    template<typename T>
        void unserialize(T& obj, FILE* fp)
    {
        throw std::exception{"δʵ�ֶ�Ӧ�ķ����л�������"};
    }

    template<> inline
        void unserialize(int64_t& obj, FILE* fp)
    {
        auto cnt = fread(&obj, sizeof(obj), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
    }

    template<> inline
        void unserialize(double& obj, FILE* fp)
    {
        auto cnt = fread(&obj, sizeof(obj), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
    }

    template<> inline
        void unserialize(std::unique_ptr<string>& obj, FILE* fp)
    {
        new (&obj) std::unique_ptr<string>(new string());//�ڷ���õ��ڴ��Ϲ�������
        stringUnserialize(fp, *obj);
    }

    // set
    // ��ʽ�� ���ͣ�1�ֽڣ� ������8�ֽڣ� ���ݣ�����C����\0�ĸ�ʽ��) --- list��set
    template<> inline
        void unserialize(std::unique_ptr<hash_set<string>>& obj, FILE* fp)
    {
        size_t sz;
        auto cnt = fread(&sz, sizeof(sz), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
        new (&obj) std::unique_ptr<hash_set<string>>(new hash_set<string>());//�ڷ���õ��ڴ��Ϲ�������
        obj->reserve(sz);
        for (size_t i = 0; i < sz; ++i)
        {
            obj->insert(stringUnserialize(fp));
        }
    }

    // list
    // ��ʽ�� ���ͣ�1�ֽڣ� ������8�ֽڣ� ���ݣ�����C����\0�ĸ�ʽ��) --- list��set
    template<> inline
        void unserialize(std::unique_ptr<deque<string>>& obj, FILE* fp)
    {
        size_t sz;
        auto cnt = fread(&sz, sizeof(sz), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
        new (&obj) std::unique_ptr<deque<string>>(new deque<string>());//�ڷ���õ��ڴ��Ϲ�������
        for (size_t i = 0; i < sz; ++i)
        {
            obj->push_back(stringUnserialize(fp));
        }
    }

    inline object objectUnserialize(FILE* fp)
    {
        unsigned char index;
        auto cnt = fread(&index, sizeof(index), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
        auto obj=object_from_index(index);
        std::visit([&](auto& e) {unserialize(e,fp); }, obj);
        return obj;
    }

    inline void dBUnserialize(objectMap& db, FILE* fp)
    {
        size_t sz;
        auto cnt = fread(&sz, sizeof(sz), 1, fp);
        if (cnt != 1)
            throw std::exception("");
        db.reserve(sz);
        for (size_t i = 0; i < sz; ++i)
        {
            auto str=stringUnserialize(fp);
            db.try_insert(std::move(str), objectUnserialize(fp));//С�ĺ���ִ��˳��
        }
        return;
    }

    inline void multiDBUnserialize(FILE* fp) noexcept
    {
        try
        {
            size_t sz;
            auto cnt = fread(&sz, sizeof(sz), 1, fp);
            if (cnt != 1 || sz > data_base_count) 
                throw std::exception("");
            //TODO:�����㹻������ݿ�
            for (size_t i = 0; i < sz; ++i)
            {
                auto& db = objectMap::getObjectMap(i);
                dBUnserialize(db, fp);
            }
        }
        catch (const std::exception& e)
        {
            if (*e.what() == '\0')
                printlogByStr("���ݿ����ݽ���ʧ�ܣ�");
            else
                printlog(e);
        }
    }
}