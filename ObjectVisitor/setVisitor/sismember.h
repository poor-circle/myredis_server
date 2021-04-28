#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, int64_t> sismember(T& obj,string& member)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> sismember(std::unique_ptr<hash_set<string>>& obj,string& member)
    {
        auto iter = obj->find(member);
        if (iter != obj->end()) {
            return myredis_succeed(1);
        }
        else
        {
            return myredis_succeed(0);
        }
    }
}
