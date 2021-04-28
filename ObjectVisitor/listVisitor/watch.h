#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"
#include "../../BaseSession.h"
namespace myredis::visitor
{
    template<typename T>
    code::status watch(T& obj, BaseSession * session)
    {
        return myredis_failed(object_type_error);
    }
    template<> inline
        code::status watch(std::unique_ptr<deque<string>>& obj, BaseSession* session)
    {
        obj->watched_session((void*)session);
        return code::status::success;
    }
}
#pragma once
