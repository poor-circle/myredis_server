#pragma once
#include "sparsepp/spp.h"
namespace myredis
{
	template<class T1,class T2>
	using hash_map = spp::sparse_hash_map<T1,T2>;
	//ʹ�����ָ�Ч�ҽ�ʡ�ڴ��hash_map(�ײ�ʹ�ÿ���Ѱַ�����hash��ͻ��
	//�������׼���unordered_map���ײ�ʹ�����������hash��ͻ��

}
