#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����ļ�������ֺ�key�йصĺ���

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


