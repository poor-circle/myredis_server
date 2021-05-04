#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, int64_t> linsert(T& obj,string& insertType,string& pivot,string& value)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> linsert(std::unique_ptr<deque<string>>& obj, string& insertType, string& pivot, string& value)
    {
        if (obj->empty()) {
            return myredis_succeed(0);
        }
        auto pivotIter = obj->begin();
        for (; pivotIter != obj->end(); pivotIter++)
        {
            if (*pivotIter == pivot) 
                break;
        }
        if (pivotIter == obj->end()) {
            return myredis_succeed(-1);
        }
        if (insertType == std::string_view("after")) {
            pivotIter++;
        }
        obj->insert(pivotIter, value);
        return myredis_succeed(obj->size());
    }
}
