#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, int64_t> sadd(T& obj,std::vector<string>& value)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> sadd(std::unique_ptr<hash_set<string>>& obj,std::vector<string>& value)
    {
        auto iter = value.begin();
        for (; iter != value.end(); iter++)
        {
            obj->insert(*iter);
        }
        return myredis_succeed(obj->size());
    }
}
