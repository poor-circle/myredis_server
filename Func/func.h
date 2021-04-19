#pragma once
#include"../stdafx.h"
#include"../BaseSession.h"
namespace myredis::func
{
	struct Ctx
	{
		std::vector<string>&& args;
		BaseSession& session;
		Ctx(std::vector<string>&& args, BaseSession& session) :
			args(std::move(args)), session(session) {}
	};

	//myredis�������ͣ�
	//����ֵ��string�����ڷ��ظ��ͻ���
	//������func::Ctx
	//��֤�����쳣�����ڲ����񣬲��׳��κ��쳣
	
	using funcPtr = std::optional<boost::container::string>(*)(Ctx&&) noexcept;

	//�����������spring boot��controller�㣬ÿһ��������Ӧһ��myredis����
	//���󲿷�����²���Ҫ����������ϸ��

	
}