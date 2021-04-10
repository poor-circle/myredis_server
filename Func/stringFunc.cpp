#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/get.h"
namespace myredis::func
{
#include"../namespace.h"

	//����һ���ַ���

	//��������ÿһ������Ӧ�þ����ܱ�֤����������

	//noexcept:ÿһ����������֤�����׳��κ��쳣����ʹ��һ������������Ҳ�ܱ�֤��Ӱ����������

	optional<string> set(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 3)
				return "-error:wrong args count\r\n";
			auto iter=getObjectMap().find(args[1]);//�������ݿ����Ƿ��ж�Ӧ��key
			if (iter == getObjectMap().end())//֮ǰû�У����ڲ���
			{
				getObjectMap().emplace(std::move(args[1]),stringToObject(std::move(args[2])));
			}
			else //ԭ�����У�ֻ��Ҫ����value����
			{
				iter->second = stringToObject(std::move(args[2]));
			}
			return "+OK\r\n";
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	//��ȡһ���ַ���
	optional<string> get(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return "-error:wrong args count\r\n";
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				return "-error:no such key\r\n";
			}
			else
			{
				string s;
				auto& ret = visit([](auto& e) {return visitor::get(e); }, iter->second).second;
				fmt::format_to
				(
					back_inserter(s),
					FMT_COMPILE("${}\r\n{}\r\n"),
					ret.size(),
					ret
				);
				return s;
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}",e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
}