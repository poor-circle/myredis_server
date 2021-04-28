#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, int64_t> srem(T& obj, std::vector<string> members)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> srem(std::unique_ptr<hash_set<string>>& obj,std::vector<string> members)
    {
        int64_t count = 0;
        for (auto iter = members.begin(); iter != members.end(); iter++) {
            count+=obj->erase(*iter);
        }
        return myredis_succeed(count);
    }
}
