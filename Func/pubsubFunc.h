#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	// test	created by lizezheng	date:2021/5/01
	//һ�����Ժ��������������пͻ��˹㲥һ����Ϣ����������wait������ȷͬ����ʾ�����޲���
	//asio::awaitable<T>��������ֵΪT��һ���첽Э�̺���
	std::optional<string> test(context&& ctx) noexcept;

	//һ�����Ժ�����ģ��ȴ�һ��������Ϣ���޲���
	//�����������Ҫ�õ�������������Ϣ���Ǿ����ó�ͬ������
	std::optional<string> wait(context&& ctx) noexcept;

	// SUBSCRIBE		creator:tigerwang	date:2021/5/3
	std::optional<string> subscribe(context&& ctx) noexcept;

	// UNSUBSCRIBE		creator:tigerwang	date:2021/5/4
	std::optional<string> unsubscribe(context&& ctx) noexcept;

	// pubsub			creator:tigerwang	date:2021/5/4
	// note:Ŀǰֻʵ��pubsub numsub
	std::optional<string> pubsub(context&& ctx) noexcept;

	// publish			creator:tigerwang	date:2021/5/5
	std::optional<string> publish(context&& ctx) noexcept;

	// psubscribe		creator:tigerwang	date:2021/5/5
	std::optional<string> psubscribe(context&& ctx) noexcept;

}