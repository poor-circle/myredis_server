#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lset 只适用于list对象
    template<typename T>
    std::pair<code::status, string&> lset(T& obj,int64_t index,string& value)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
        std::pair<code::status, string&> lset(std::unique_ptr<deque<string>>& obj,int64_t index,string& value)
    {

        if (obj->empty()) {
            return myredis_failed(object_is_empty);
        }

        const int64_t listLen = obj->size();
        if (index < 0) {
            index = listLen + index;
        }
        if (index >= 0 && index < listLen) {
            static string changeContent;
            changeContent = value;
            obj->at(index) = changeContent;
            return myredis_succeed(changeContent);
        }
        else 
        {
            return myredis_failed(index_out_of_range);

        }
    }
}
