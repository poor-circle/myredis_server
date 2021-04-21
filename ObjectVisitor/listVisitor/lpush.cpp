#include "stdafx.h"
#include "lpush.h"

namespace myredis::visitor {
    // lpush ֻ������list����
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
        // ͷ������
        value->push_front(pushContent);
        // ���ظ���
        return myredis_succeed(value->size());
    }
}