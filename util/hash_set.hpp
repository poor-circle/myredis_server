#pragma once
#include "sparsepp/spp.h"
namespace myredis
{
	template<class T>
	using hash_set = spp::sparse_hash_set<T>;
	//ʹ�����ָ�Ч�ҽ�ʡ�ڴ��hash_set(�ײ�ʹ�ÿ���Ѱַ�����hash��ͻ��
	//�������׼���unordered_set���ײ�ʹ�����������hash��ͻ��
}