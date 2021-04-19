#include "stdafx.h"
#include "connectFunc.h"
#include "../object.hpp"
#include "../code.h"
namespace myredis::func
{
#include"../namespace.i"
	//ping
	//author: lizezheng
	//date: 2021/04/19
	std::optional<string> ping(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		try
		{
			if (args.size() == 1)
				return code::pong;
			else if (args.size() == 2)
				return code::getSingleReply(args[1]);
			return code::args_count_error;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}
	//select (choose database by index)
	//args：index (0<=index<limit)
	//author: lizezheng
	//date: 2021/04/19
	std::optional<string> select(context&& ctx) noexcept
	{
		try
		{
			if (ctx.args.size() != 2)
				return code::args_count_error;
			int64_t id;
			if (!try_lexical_convert(ctx.args[1], id) || id < 0 || id >= data_base_count)
				return code::database_index_error;
			ctx.session.setDataBaseID(id);
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}
	//auth (for login)
	//args：password
	//author: lizezheng
	//date: 2021/04/19
	std::optional<string> auth(context&& ctx) noexcept
	{
		try
		{
			if (ctx.args.size() != 2)
			{
				return code::args_count_error;
			}
			if (myredis_password == ""sv)
				return code::no_password_error;
			ctx.session.setLogined(ctx.args[1] == myredis_password);
			return ctx.session.isLogined() ? code::ok : code::password_wrong_error;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}
	//echo (just reply what client send)
	//args：string
	//author: lizezheng
	//date: 2021/04/19
	std::optional<string> echo(context&& ctx) noexcept
	{
		try
		{
			if (ctx.args.size() != 2)
			{
				return code::args_count_error;
			}
			return code::getBulkReply(ctx.args[1]);
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}
	std::optional<string> quit(context&& ctx) noexcept
	{
		ctx.session.setClosed();
		return nullopt;
	}
}
