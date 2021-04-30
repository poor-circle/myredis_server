#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lpush 只适用于list对象
    template<typename T>
    std::pair<code::status, int64_t> lpush(T& value, std::vector<string>& pushContent,string key,BaseSession& session) 
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, std::vector<string>& pushContent, string key, BaseSession& session)
    {
        if (value->empty())//添加一条信息，该信息将用于唤醒阻塞在该队列上的会话（如果有的话）
        {
            session.wake_up_queue.emplace(std::move(key), session.getDataBaseID());
        }

        for(auto iter = pushContent.begin();iter!=pushContent.end();iter++)
            //  省去移动复制元素的过程
            value->emplace_front(*iter);

        // 返回个数
        return myredis_succeed(value->size());
    }

    // rpush 只适用于list对象
    template<typename T>
    std::pair<code::status, int64_t> rpush(T& value, std::vector<string>& pushContent, string key, BaseSession& session)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> rpush(std::unique_ptr<deque<string>>& value, std::vector<string>& pushContent, string key, BaseSession& session)
    {
        if (value->empty())//添加一条信息，该信息将用于唤醒阻塞在该队列上的会话（如果有的话）
        {
            session.wake_up_queue.emplace(std::move(key), session.getDataBaseID());
        }
        for (auto iter = pushContent.begin(); iter != pushContent.end(); iter++)
            //  省去移动复制元素的过程
            value->emplace_back(*iter);
        // 返回个数
        return myredis_succeed(value->size());
    }
}
