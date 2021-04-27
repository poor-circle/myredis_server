#pragma once
#include"../stdafx.h"
#include "../BaseSession.h"
namespace myredis::func
{
	//myredis�������ͣ�
	//����ֵ��string�����ڷ��ظ��ͻ���
	//������һ���������ͣ�ָ��һ��vector<string>
	//��֤�����쳣�����ڲ����񣬲��׳��κ��쳣
	
	struct context//����������
	{
		std::vector<string>&& args;//��������ݵĲ���
		BaseSession& session;//���ӵ�����
		context(std::vector<string>&& args,BaseSession& session):
			args(std::move(args)),session(session){}
	};

	using funcPtr = std::optional<boost::container::string>(*)(context&& ctx) noexcept;

	enum funcType
	{
		read,
		write,
		connect
	};
	struct funcInfo
	{
		funcPtr func;//����ָ��
		funcType type;//��������
	};


	//�����������spring boot��controller�㣬ÿһ��������Ӧһ��myredis����
	//���󲿷�����²���Ҫ����������ϸ��
}