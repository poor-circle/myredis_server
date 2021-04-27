#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//	sadd		creator:tigerwang	date:2021/4/27
	std::optional<string> sadd(context&& ctx) noexcept;
	//	scard		creator:tigerwang	date:2021/4/27
	std::optional<string> scard(context&& ctx) noexcept;
	//	smembers	creator:tigerwang	date:2021/4/27
	std::optional<string> smembers(context&& ctx) noexcept;
}


