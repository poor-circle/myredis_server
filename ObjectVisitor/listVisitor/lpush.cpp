#include "stdafx.h"
#include "lpush.h"

namespace myredis::visitor {
    // lpush 只适用于list对象
    std::pair<code::status, int64_t> lpush(int64_t& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> lpush(double& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> lpush(std::unique_ptr<string>& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> lpush(std::unique_ptr<hash_set<string>>& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> lpush(std::unique_ptr<hash_map<string, string>>& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> lpush(std::unique_ptr<key_ordered_map<double, string>>& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, string& pushContent) {
        // 头部推入
        value->push_front(pushContent);
        // 返回个数
        return myredis_succeed(value->size());
    }
}