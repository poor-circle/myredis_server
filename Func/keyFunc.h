#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//在这里声明各种函数类型

	//这个文件负责各种和key有关的函数

	//	exists		created by tigerwang	date:2021/4/19
	std::optional<string> exists(std::vector<string>&& args) noexcept;
}
