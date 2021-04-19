#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//在这里声明各种函数类型

	//这个文件负责各种和string有关的函数

	//测试服务器连接
	std::optional<string> ping(std::vector<string>&& args) noexcept;
	//关闭服务器连接
	std::optional<string> quit(std::vector<string>&& args) noexcept;
}