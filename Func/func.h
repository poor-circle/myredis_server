#pragma once
#include"../stdafx.h"
namespace myredis::func
{
	//myredis�������ͣ�
	//����ֵ��string�����ڷ��ظ��ͻ���
	//������һ���������ͣ�ָ��һ��vector<string>
	//��֤�����쳣�����ڲ����񣬲��׳��κ��쳣
	
	using funcPtr = std::optional<boost::container::string>(*)(std::vector<boost::container::string>&&) noexcept;

	//�����������spring boot��controller�㣬ÿһ��������Ӧһ��myredis����
	//���󲿷�����²���Ҫ����������ϸ��
}