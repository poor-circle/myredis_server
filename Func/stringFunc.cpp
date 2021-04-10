#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/get.h"
namespace myredis::func
{
#include"../namespace.h"

	//设置一个字符串

	//无阻塞：每一个函数应该尽可能保证不出现阻塞

	//noexcept:每一个函数都保证自身不抛出任何异常。即使有一个操作崩溃，也能保证不影响其他操作

	optional<string> set(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 3)
				return "-error:wrong args count\r\n";
			auto iter=getObjectMap().find(args[1]);//查找数据库中是否有对应的key
			if (iter == getObjectMap().end())//之前没有，现在插入
			{
				getObjectMap().emplace(std::move(args[1]),stringToObject(std::move(args[2])));
			}
			else //原来就有，只需要更新value即可
			{
				iter->second = stringToObject(std::move(args[2]));
			}
			return "+OK\r\n";
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}

	//获取一个字符串
	optional<string> get(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return "-error:wrong args count\r\n";
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//找不到对应的key
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
			return nullopt;//返回空值
		}
	}
}