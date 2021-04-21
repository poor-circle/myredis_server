#include "stdafx.h"
#include "lrange.h"

namespace myredis::visitor {
    // lrange ֻ������list����
    // TODO:lrange��ûд��
    std::pair<code::status, int64_t> lpush(int64_t& value, string& pushContent) {
        return { code::status::object_type_error,0 };
    }
    std::pair<code::status, string&> lrange(int64_t& value, int64_t start, int64_t end) {

    }
    std::pair<code::status, int64_t> lrange(double& value, int64_t start, int64_t end);
    std::pair<code::status, int64_t> lrange(std::unique_ptr<string>& value, int64_t start, int64_t end);
    std::pair<code::status, int64_t> lrange(std::unique_ptr<hash_set<string>>& value, int64_t start, int64_t end);
    std::pair<code::status, int64_t> lrange(std::unique_ptr<hash_map<string, string>>& value, int64_t start, int64_t end);
    std::pair<code::status, int64_t> lrange(std::unique_ptr<key_ordered_map<double, string>>& value, int64_t start, int64_t end);
    std::pair<code::status, int64_t> lrange(std::unique_ptr<deque<string>>& value, int64_t start, int64_t end);
    std::pair<code::status, int64_t> lpush(std::unique_ptr<deque<string>>& value, string& pushContent) {
        // ͷ������
        value->push_front(pushContent);
        // ���ظ���
        return myredis_succeed(value->size());
    }
}