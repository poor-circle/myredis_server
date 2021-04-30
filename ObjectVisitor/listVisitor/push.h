#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    // lpush ֻ������list����
    template<typename T>
    std::pair<code::status, int64_t> lpush(T& value, std::vector<string>& pushContent,string key,BaseSession& session) 
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, std::vector<string>& pushContent, string key, BaseSession& session)
    {
        if (value->empty())//���һ����Ϣ������Ϣ�����ڻ��������ڸö����ϵĻỰ������еĻ���
        {
            session.wake_up_queue.emplace(std::move(key), session.getDataBaseID());
        }

        for(auto iter = pushContent.begin();iter!=pushContent.end();iter++)
            //  ʡȥ�ƶ�����Ԫ�صĹ���
            value->emplace_front(*iter);

        // ���ظ���
        return myredis_succeed(value->size());
    }

    // rpush ֻ������list����
    template<typename T>
    std::pair<code::status, int64_t> rpush(T& value, std::vector<string>& pushContent, string key, BaseSession& session)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> rpush(std::unique_ptr<deque<string>>& value, std::vector<string>& pushContent, string key, BaseSession& session)
    {
        if (value->empty())//���һ����Ϣ������Ϣ�����ڻ��������ڸö����ϵĻỰ������еĻ���
        {
            session.wake_up_queue.emplace(std::move(key), session.getDataBaseID());
        }
        for (auto iter = pushContent.begin(); iter != pushContent.end(); iter++)
            //  ʡȥ�ƶ�����Ԫ�صĹ���
            value->emplace_back(*iter);
        // ���ظ���
        return myredis_succeed(value->size());
    }
}
