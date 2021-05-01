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

	using syncFuncPtr = std::optional<boost::container::string>(*)(context&& ctx) noexcept;//ͬ��api�ĺ���ָ��
	using asyncFuncPtr = asio::awaitable<std::optional<boost::container::string>>(*)(context&& ctx) noexcept;//�첽api�ĺ���ָ��

	enum class funcType
	{
		read,
		write,
		connect,
		blocked,
		pubsub,
		async_pubsub,
	};
	union funcPtr
	{
		syncFuncPtr syncFunc;//ͬ������ָ��
		asyncFuncPtr asyncFunc;//�첽����ָ��
	};
	struct funcInfo
	{
		syncFuncPtr syncptr;
		asyncFuncPtr asyncptr;//����֮���Բ�����union���ͣ�����Ϊ���ƺ��޷�����Э�̺��������
		funcType type;//��������
		bool isAsyncFunc(void) const noexcept
		{
			return type == funcType::async_pubsub;
		}
		funcInfo(syncFuncPtr ptr, funcType type):syncptr(ptr),asyncptr(nullptr),type(type){}
		funcInfo(asyncFuncPtr ptr, funcType type) :syncptr(nullptr), asyncptr(ptr), type(type) {}
	};


	//�����������spring boot��controller�㣬ÿһ��������Ӧһ��myredis����
	//���󲿷�����²���Ҫ����������ϸ��
}