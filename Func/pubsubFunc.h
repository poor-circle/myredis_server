#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	// test	created by lizezheng	date:2021/5/01
	//һ�����Ժ��������������пͻ��˹㲥һ����Ϣ����������wait������ȷͬ����ʾ�����޲���
	//���������Ҫ�õ�������������Ϣ���Ǿ����ó��첽����
	//asio::awaitable<T>����������ֵΪT��һ���첽Э�̺���
	asio::awaitable<std::optional<string>> test(context&& ctx) noexcept;

	//һ�����Ժ�����ģ��ȴ�һ��������Ϣ���޲���
	//�����������Ҫ�õ�������������Ϣ���Ǿ����ó�ͬ������
	std::optional<string> wait(context&& ctx) noexcept;
}