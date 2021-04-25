#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, string&> lindex(T& obj, int64_t index)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
        std::pair<code::status, string&> lindex(std::unique_ptr<deque<string>>& obj,int64_t index)
    {
        if (index < 0) 
        {
            index += obj->size();
        }
        if (index >= 0 && index < obj->size()) {
            return myredis_succeed(obj->at(index));
        }
        else
            return myredis_failed(index_out_of_range);
    }
}
