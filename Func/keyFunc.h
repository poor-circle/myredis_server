#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����ļ�������ֺ�key�йصĺ���

	//	exists		created by tigerwang	date:2021/4/19
	std::optional<string> exists(context&& ctx) noexcept;
	//	del		created by lizezheng	date:2021/4/20
	std::optional<string> del(context&& ctx) noexcept;
	//	keys	created by lizezheng	date:2021/4/20
	std::optional<string> keys(context&& ctx) noexcept;
	//	rename	created by lizezheng	date:2021/4/20
	std::optional<string> rename(context&& ctx) noexcept;
	//	renamenx	created by lizezheng	date:2021/4/20
	std::optional<string> renamenx(context&& ctx) noexcept;
}
