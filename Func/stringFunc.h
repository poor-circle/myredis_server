#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����ļ�������ֺ�string�йصĺ���

	//����һ���ַ���
	std::optional<string> set(std::vector<string>&& args) noexcept;
	//��ȡһ���ַ���
	std::optional<string> get(std::vector<string>&& args) noexcept;
	//��������Ӹ���ĺ���
	std::optional<string> ping(std::vector<string>&& args) noexcept;
	//	append		created by tigerwang	date:2021/4/18
	std::optional<string> append(std::vector<string>&& args) noexcept;
	//	strlen		created by tigerwang	date:2021/4/18
	std::optional<string> strlen(std::vector<string>&& args) noexcept;
	//	getrange	created by tigerwang	date:2021/4/18
	std::optional<string> getrange(std::vector<string>&& args) noexcept;
	//	setnx		created by tigerwang	date:2021/4/18
	std::optional<string> setnx(std::vector<string>&& args) noexcept;
	//	getset		created by tigerwang	date:2021/4/18
	std::optional<string> getset(std::vector<string>&& args) noexcept;
	//	msetnx		created by tigerwang	date:2021/4/19
	std::optional<string> msetnx(std::vector<string>&& args) noexcept;
	//	incre		created by tigerwang	date:2021/4/19
	std::optional<string> incr(std::vector<string>&& args) noexcept;

}