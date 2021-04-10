#pragma once
#include "boost/container/string.hpp"
#include "boost/container_hash/hash.hpp"
#include "boost/algorithm/string.hpp"
namespace myredis
{
	using string = boost::container::string;
    //boost的string类型具有更好的sso优化能力（支持存储size()-1个字符），且大小更小（仅3个指针）
    //std::string更大，sso优化效果更差，自带算法较少
    //需要注意，本项目中所有没指明命名空间的string均为boost::container::string
}
namespace std
{
    template<>
    struct hash<myredis::string>
    {
        std::size_t operator()(myredis::string const& p) const
        {
            return boost::container::hash_value(p);
        }
    };
    //添加标准库对于myredis::string的哈希支持
}