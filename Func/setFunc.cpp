#include "stdafx.h"
#include "setFunc.h"
#include "../object.hpp"
#include "../code.h"
#include "../ObjectVisitor/setVisitor/sadd.h"
#include "../ObjectVisitor/setVisitor/scard.h"
#include "../ObjectVisitor/setVisitor/smembers.h"
#include "../ObjectVisitor/setVisitor/sismember.h"
#include "../ObjectVisitor/setVisitor/srem.h"




namespace myredis::func
{
#include"../namespace.i"

	/*
	* sadd key member [member...]
	* @author:tigerwang
	* date:2021/4/27
	*/
	std::optional<string> sadd(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			vector<string> addContent;
			for (int i = 2; i < args.size(); i++) {
				addContent.push_back(args[i]);
			}
			if (iter == objectMap.end())
			{
				// 如果没找到,则新建一个空列表,并在头部插入
				auto set = std::make_unique<hash_set<string>>();
				auto ret = visitor::sadd(set, addContent);
				objectMap.update(std::move(args[1]), std::move(set));
				return code::getIntegerReply(ret.second);
			}
			else
			{
				auto ret = visit([&addContent](auto& e)
				{
					return visitor::sadd(e, addContent);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* scard key 
	* @author:tigerwang
	* date:2021/4/27
	*/
	std::optional<string> scard(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::scard(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;
		}
	}
	
	/*
	* smembers key
	* @author:tigerwang
	* date:2021/4/27
	*/
	std::optional<string> smembers(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::key_error;
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::smembers(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::getMultiReplyByRange(ret.second.begin(), ret.second.end(),
					[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{
					code::getBulkReplyTo(*arg, s);
					return 1;
				});
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;
		}
	}

	/*
	* sismember key member
	* @author:tigerwang
	* date:2021/4/28
	*/
	std::optional<string> sismember(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::getIntegerReply(0);

			}
			else
			{
				string member = args[2];
				auto ret = visit([&member](auto& e)
				{
					return visitor::sismember(e,member);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;
		}
	}

	/*
	* srem key member [member...]
	* @author:tigerwang
	* date:2021/4/28
	*/
	std::optional<string> srem(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			vector<string> remContent;
			for (int i = 2; i < args.size(); i++) {
				remContent.push_back(args[i]);
			}
			if (iter == objectMap.end())
			{
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&remContent](auto& e)
				{
					return visitor::srem(e, remContent);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

}
