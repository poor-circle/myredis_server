#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//在这里声明各种函数类型

	//这个文件负责各种和string有关的函数

	//设置一个字符串
	std::optional<string> set(std::vector<string>&& args) noexcept;
	//获取一个字符串
	std::optional<string> get(std::vector<string>&& args) noexcept;
	//在下面添加更多的函数
	std::optional<string> ping(std::vector<string>&& args) noexcept;
	//append
	std::optional<string> append(std::vector<string>&& args) noexcept;
}