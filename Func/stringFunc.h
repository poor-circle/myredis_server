#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����һ���ַ���
	std::optional<string> set(std::vector<string>&& args) noexcept;
	//��ȡһ���ַ���
	std::optional<string> get(std::vector<string>&& args) noexcept;

}