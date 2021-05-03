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
	//	object	created by lizezheng	date:2021/4/20
	std::optional<string> object(context&& ctx) noexcept;
	//	sort	created by lizezheng	date:2021/4/21
	// TODO:�ȴ�ʵ�������������ݽṹ
	std::optional<string> sort(context&& ctx) noexcept;
	//	dump	created by lizezheng	date:2021/4/21
	// TODO:�ȴ������л���ʵ��
	std::optional<string> dump(context&& ctx) noexcept;
	//	type	created by lizezheng	date:2021/4/21
	std::optional<string> type(context&& ctx) noexcept;
	//	scan	created by lizezheng	date:2021/4/21
	std::optional<string> scan(context&& ctx) noexcept;
	// expire created by lizezheng data:2021/05/03
	std::optional<string> expire(context&& ctx) noexcept;
}
