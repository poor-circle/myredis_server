#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis::func
{
	//�������������ֺ�������

	//����ļ�������ֺ�key�йصĺ���

	//	exists		created by tigerwang	date:2021/4/19
	std::optional<string> exists(context&& ctx) noexcept;
}
