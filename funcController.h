#pragma once
#include "stdafx.h"
#include "code.h"
#include "Func/func.h"
namespace myredis::func
{
	asio::awaitable<void> funcControll(func::context&& ctx,string &ret);

	asio::awaitable<std::optional<string>>  funcRunner(func::funcInfo foo, context&& ctx);
}
