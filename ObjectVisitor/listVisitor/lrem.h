#pragma once
#include "../../stdafx.h"
#include "../../code.h"
#include "../../object.hpp"

namespace myredis::visitor
{
    template<typename T>
    std::pair<code::status, int64_t> lrem(T& obj,int64_t count,string& value)
    {
        return { code::status::object_type_error,0 };
    }
    template<> inline
        std::pair<code::status, int64_t> lrem(std::unique_ptr<deque<string>>& obj, int64_t count, string& value)
    {
        int delCount = 0;
        // >0 从头到尾
        if (count > 0) 
        {
            for (auto iter = obj->begin(); iter != obj->end(); ) {
                if (*iter == value) {
                    iter = obj->erase(iter);
                    delCount++;
                    if (delCount == count) {
                        break;
                    }
                }
                else {
                    iter++;
                }
            }
        }
        // =0删除所有的value
        if (count == 0) 
        {
            for (auto iter = obj->begin(); iter != obj->end();) {
                if (*iter == value) {
                    iter = obj->erase(iter);
                    delCount++;
                }
                else {
                    iter++;
                }
            }
        }
        // <0从尾到头删除 
        // 详见https://blog.csdn.net/u011391040/article/details/50433237
        if (count < 0) 
        {
            count = -count;
            for (auto iter = obj->rbegin(); iter != obj->rend(); ) {
                if (*iter == value) {
                    iter = deque<string>::reverse_iterator(obj->erase((++iter).base()));
                    delCount++;
                    if (delCount == count) {
                        break;
                    }
                }
                else {
                    iter++;
                }
            }
        }
        return myredis_succeed(delCount);
    }
}
