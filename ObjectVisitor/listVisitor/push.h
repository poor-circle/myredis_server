#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lpush ֻ������list����
    template<typename T>
    std::pair<code::status, int64_t> lpush(T& value, string&& pushContent,string key,BaseSession& session) 
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, string&& pushContent, string key, BaseSession& session)
    {
        if (value->empty())//���һ����Ϣ������Ϣ�����ڻ��������ڸö����ϵĻỰ������еĻ���
        {
            session.wake_up_queue.emplace(std::move(key), session.getDataBaseID());
        }

        //  ʡȥ�ƶ�����Ԫ�صĹ���
        value->emplace_front(pushContent);


        // ���ظ���
        return myredis_succeed(value->size());
    }

    // rpush ֻ������list����
    template<typename T>
    std::pair<code::status, int64_t> rpush(T& value, string&& pushContent, string key, BaseSession& session)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> rpush(std::unique_ptr<deque<string>>& value, string&& pushContent, string key, BaseSession& session)
    {
        if (value->empty())//���һ����Ϣ������Ϣ�����ڻ��������ڸö����ϵĻỰ������еĻ���
        {
            session.wake_up_queue.emplace(std::move(key), session.getDataBaseID());
        }
        value->emplace_back(pushContent);
        // ���ظ���
        return myredis_succeed(value->size());
    }
}
