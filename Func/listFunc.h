#pragma once
#include "../stdafx.h"
#include "func.h"

namespace myredis::func
{
	//这个文件负责各种和list有关的函数

	//	lpush		created by tigerwang	date:2021/4/21
	std::optional<string> lpush(context&& ctx) noexcept;
	//	lpushx		created by tigerwang	date:2021/4/21
	std::optional<string> lpushx(context&& ctx) noexcept;
	//	llen		created by tigerwang	date:2021/4/21
	std::optional<string> llen(context&& ctx) noexcept;

}
