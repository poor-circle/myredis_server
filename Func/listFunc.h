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
	//	lrange		created by tigerwang	date:2021/4/22
	std::optional<string> lrange(context&& ctx) noexcept;
	//	rpush		created by tigerwang	date:2021/4/23
	std::optional<string> rpush(context&& ctx) noexcept;
	//	rpushx		created by tigerwang	date:2021/4/23
	std::optional<string> rpushx(context&& ctx) noexcept;
	//	lpop		created by tigerwang	date:2021/4/23
	std::optional<string> lpop(context&& ctx) noexcept;
	//	rpop		created by tigerwang	date:2021/4/23
	std::optional<string> rpop(context&& ctx) noexcept;
	//	rpoplpush	created by tigerwang	date:2021/4/23
	std::optional<string> rpoplpush(context&& ctx) noexcept;

}
