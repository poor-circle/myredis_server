#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, std::vector<string> > smembers(T& obj)
    {
        return { code::status::object_type_error,std::vector<string>()};
    }
    template<> inline
        std::pair<code::status,std::vector<string> > smembers(std::unique_ptr<hash_set<string>>& obj)
    {
        auto set = std::vector<string>();
        copy(obj->begin(), obj->end(), std::back_inserter(set));
        return myredis_succeed(set);
    }
}
