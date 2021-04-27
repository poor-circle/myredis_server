#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, int64_t> scard(T& obj)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> scard(std::unique_ptr<hash_set<string>>& obj)
    {
        return myredis_succeed(obj->size());
    }
}
