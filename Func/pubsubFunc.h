#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	// test	created by lizezheng	date:2021/5/01
	//一个测试函数，向其他所有客户端广播一条消息（事先输入wait才能正确同步显示），无参数
	asio::awaitable<std::optional<string>> test(context&& ctx) noexcept;

	//一个测试函数，模拟等待一条订阅消息，无参数
	std::optional<string> wait(context&& ctx) noexcept;
}