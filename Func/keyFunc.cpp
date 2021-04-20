#include "stdafx.h"
#include "func.h"
#include "keyFunc.h"
#include "../object.hpp"
#include "../code.h"

//func层相当于spring boot的controller层
//通过controller调用visitor层
//调用格式：visit([](auto& e) {return visitor::funcName(e); }, object)
//visitor::funcName：你要调用的函数名
//object：你要操作的对象类型
namespace myredis::func
{
#include"../namespace.i"

	//设置一个字符串

	//无阻塞：每一个函数应该尽可能保证不出现阻塞

	//noexcept:每一个函数都保证自身不抛出任何异常。即使有一个操作崩溃，也能保证不影响其他操作
	/*
	* @author: tigerwang
	* date:2021/4/19
	* exists key [key ...]
	* 返回key是否存在。
	* 返回值
	* integer-reply，如下的整数结果
	* 1 如果key存在
	* 0 如果key不存在
	*/
	std::optional<string> exists(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
			{
				for (auto iter=args.begin()+1;iter!=args.end();++iter)
				{
					auto ans = objectMap.find(*iter);
					if (ans == objectMap.end())
						return code::getIntegerReply(0);
				}
				return code::getIntegerReply(1);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	std::optional<string> del(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
			{
				int64_t cnt = 0;
				while (args.size()>1)
				{
					auto ans = objectMap.erase(std::move(args.back()));
					args.pop_back();
					if (ans) ++cnt;
				}
				return code::getIntegerReply(cnt);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	std::optional<string> keys(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			else
			{
				std::regex rx;
				try
				{
					auto flag = regex_constants::ECMAScript ;
					if (objectMap.size() >= regexOpLowerBound)
						flag |= regex_constants::syntax_option_type::optimize;
					rx=regex(args[1].c_str(), flag);
				}
				catch (const exception &e)
				{
					return code::regex_error;
				}
				return code::getMultiReply(objectMap.begin(), objectMap.end(),
					[&rx](hash_map<keyIter, object>::iterator iter, auto s)
					{
						if (std::regex_match((*iter->first.iter).c_str(), rx))
						{
							code::getBulkReplyTo(*iter->first.iter, s);
							return 1;
						}
						else 
							return 0;
					});
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}


}