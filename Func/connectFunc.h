#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����ļ�������ֺ�string�йصĺ���

	//���Է���������
	std::optional<string> ping(std::vector<string>&& args) noexcept;
	//�رշ���������
	std::optional<string> quit(std::vector<string>&& args) noexcept;
}