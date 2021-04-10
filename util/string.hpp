#pragma once
#include "boost/container/string.hpp"
#include "boost/container_hash/hash.hpp"
#include "boost/algorithm/string.hpp"
namespace myredis
{
	using string = boost::container::string;
    //boost��string���;��и��õ�sso�Ż�������֧�ִ洢size()-1���ַ������Ҵ�С��С����3��ָ�룩
    //std::string����sso�Ż�Ч������Դ��㷨����
    //��Ҫע�⣬����Ŀ������ûָ�������ռ��string��Ϊboost::container::string
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
    //��ӱ�׼�����myredis::string�Ĺ�ϣ֧��
}