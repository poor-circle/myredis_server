#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{

	// SUBSCRIBE		creator:tigerwang	date:2021/5/3
	std::optional<string> subscribe(context&& ctx) noexcept;

	// UNSUBSCRIBE		creator:tigerwang	date:2021/5/4
	std::optional<string> unsubscribe(context&& ctx) noexcept;

	// pubsub			creator:tigerwang	date:2021/5/4
	// note:目前只实现pubsub numsub
	std::optional<string> pubsub(context&& ctx) noexcept;

	// publish			creator:tigerwang	date:2021/5/5
	std::optional<string> publish(context&& ctx) noexcept;

	// psubscribe		creator:tigerwang	date:2021/5/5
	std::optional<string> psubscribe(context&& ctx) noexcept;

	// punsubscribe		creator:tigerwang	date:2021/5/6
	std::optional<string> punsubscribe(context&& ctx) noexcept;

	//helper func,creator:lizezheng	date:2021/5/6
	bool isAllowWhenSubscribed(const string& cmd);


}