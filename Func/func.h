#pragma once
#include"../stdafx.h"
namespace myredis::func
{
	//redis�������ͣ�
	//����ֵ��string�����ڷ��ظ��ͻ���
	//������һ���������ͣ�ָ��һ��vector<string>
	//��֤�����쳣�����ڲ����񣬲��׳��κ��쳣
	
	using funcPtr = std::optional<boost::container::string>(*)(std::vector<boost::container::string>&&) noexcept;
}