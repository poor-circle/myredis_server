#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//在这里声明各种函数类型

	//这个文件负责各种和key有关的函数

	//	ping		created by lizezheng	date:2021/4/19
	std::optional<string> ping(context&& ctx) noexcept;
	//	select		created by lizezheng	date:2021/4/19
	std::optional<string> select(context&& ctx) noexcept;
	//	auth		created by lizezheng	date:2021/4/19
	std::optional<string> auth(context&& ctx) noexcept;
	//	echo		created by lizezheng	date:2021/4/19
	std::optional<string> echo(context&& ctx) noexcept;
	//	quit		created by lizezheng	date:2021/4/19
	std::optional<string> quit(context&& ctx) noexcept;
}


