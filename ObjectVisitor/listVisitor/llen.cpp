#include "stdafx.h"
#include "lpush.h"

namespace myredis::visitor {
    // lpush 只适用于list对象
    std::pair<code::status, int64_t> llen(int64_t& value) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> llen(double& value) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> llen(std::unique_ptr<string>& value) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> llen(std::unique_ptr<hash_set<string>>& value) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> llen(std::unique_ptr<hash_map<string, string>>& value) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> llen(std::unique_ptr<key_ordered_map<double, string>>& value) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, int64_t> llen(std::unique_ptr<deque<string>>& value) {
        // 返回个数
        return myredis_succeed(value->size());
    }
}