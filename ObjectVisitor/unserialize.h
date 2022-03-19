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
        throw std::exception{"未实现对应的反序列化函数！"};
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
        new (&obj) std::unique_ptr<string>(new string());//在分配好的内存上构建对象
        stringUnserialize(fp, *obj);
    }

    // set
    // 格式： 类型（1字节） 个数（8字节） 数据（按照C语言\0的格式存) --- list、set
    template<> inline
        void unserialize(std::unique_ptr<hash_set<string>>& obj, FILE* fp)
    {
        size_t sz;
        auto cnt = fread(&sz, sizeof(sz), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
        new (&obj) std::unique_ptr<hash_set<string>>(new hash_set<string>());//在分配好的内存上构建对象
        obj->reserve(sz);
        for (size_t i = 0; i < sz; ++i)
        {
            obj->insert(stringUnserialize(fp));
        }
    }

    // list
    // 格式： 类型（1字节） 个数（8字节） 数据（按照C语言\0的格式存) --- list、set
    template<> inline
        void unserialize(std::unique_ptr<deque<string>>& obj, FILE* fp)
    {
        size_t sz;
        auto cnt = fread(&sz, sizeof(sz), 1, fp);
        if (cnt != 1) 
            throw std::exception("");
        new (&obj) std::unique_ptr<deque<string>>(new deque<string>());//在分配好的内存上构建对象
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
            db.try_insert(std::move(str), objectUnserialize(fp));//小心函数执行顺序！
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
            //TODO:申请足够多的数据库
            for (size_t i = 0; i < sz; ++i)
            {
                auto& db = objectMap::getObjectMap(i);
                dBUnserialize(db, fp);
            }
        }
        catch (const std::exception& e)
        {
            if (*e.what() == '\0')
                printlogByStr("数据库内容解析失败！");
            else
                printlog(e);
        }
    }
}