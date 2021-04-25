#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, string& > ltrim(T& obj, int64_t start, int64_t end)
    {
        return myredis_failed(object_type_error);
    }

    template<> inline
        std::pair<code::status, string& > ltrim(std::unique_ptr<deque<string>>& obj, int64_t start, int64_t end)
    {

        const int64_t strLen = obj->size();
        string temp;
        // 如果小于零 + strLen ，还小于0说明超过长度了，删完了
        if (start > strLen)
        {
            obj->clear();
            return myredis_succeed(temp);
        }


        if (start < 0)
            start = strLen + start;
        if (start < 0)
            start = 0;
        if (end < 0)
            end = strLen + end;
        if (end < 0)
            end = 0;
        if (end > strLen)
            end = strLen - 1;
        if (end < start)
        {
            obj->clear();
            return myredis_succeed(temp);
        }
        auto list = std::make_unique<deque<string>>();
        copy(obj->begin() + start, obj->begin() + end + 1, std::back_inserter(*list));
        obj = std::move(list);
        //obj->erase(obj->begin(), obj->begin() + start);
        //obj->erase(obj->end(),obj->end())
        return myredis_succeed(temp);

    }
}
