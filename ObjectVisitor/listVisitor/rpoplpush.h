#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{

    // rpoplpush 只适用于list对象
    template<typename T>
    std::pair<code::status, string&> rpoplpush(T& source, object& des)
    {
        return myredis_failed(object_type_error);
    }

    template<> inline
        std::pair<code::status, string&> rpoplpush(std::unique_ptr<deque<string>>& source, object& des)
    {
        if(source->empty())
        {
            return myredis_failed(object_is_empty);
        }
        if (auto pval = std::get_if<std::unique_ptr<deque<string>>>(&des)) {
            static string temp;
            temp = source->back();
            source->pop_back();

            (*pval)->emplace_front(temp);

            return myredis_succeed(temp);
        }
        else 
        {
            return myredis_failed(object_type_error);
        }
    }
}
