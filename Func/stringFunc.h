#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����ļ�������ֺ�string�йصĺ���

	//����һ���ַ���
	std::optional<string> set(context&& ctx) noexcept;
	//��ȡһ���ַ���
	std::optional<string> get(context&& ctx) noexcept;
	//	sgetrange	created by lizezheng	date:2021/4/18
	std::optional<string> setrange(context&& ctx) noexcept;
	//	mset		created by lizezheng	date:2021/4/18
	std::optional<string> mset(context&& ctx) noexcept;
	//	mget		created by lizezheng	date:2021/4/18
	std::optional<string> mget(context&& ctx) noexcept;
	//	created by tigerwang	date:2021/4/18
	std::optional<string> append(context&& ctx) noexcept;
	//	strlen		created by tigerwang	date:2021/4/18
	std::optional<string> strlen(context&& ctx) noexcept;
	//	getrange	created by tigerwang	date:2021/4/18
	std::optional<string> getrange(context&& ctx) noexcept;
	//	setnx		created by tigerwang	date:2021/4/18
	std::optional<string> setnx(context&& ctx) noexcept;
	//	getset		created by tigerwang	date:2021/4/18
	std::optional<string> getset(context&& ctx) noexcept;
	//	msetnx		created by tigerwang	date:2021/4/19
	std::optional<string> msetnx(context&& ctx) noexcept;
	//	incr		created by tigerwang	date:2021/4/19
	std::optional<string> incr(context&& ctx) noexcept;
	//	incrby		created by tigerwang	date:2021/4/20
	std::optional<string> incrby(context&& ctx) noexcept;
}