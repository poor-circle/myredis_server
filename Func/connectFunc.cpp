#include "stdafx.h"
#include "connectFunc.h"
#include "../object.hpp"
#include "../code.h"
#include "../multiServer/SonServer.h"
#include "../multiServer/FatherServer.h"
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
			printlog(e);
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
			printlog(e);
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
			printlog(e);
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
			printlog(e);
			return nullopt;//返回空值
		}
	}
	std::optional<string> quit(context&& ctx) noexcept
	{
		ctx.session.setClosed();
		return "";
	}
	//slave ( reply the ip and port of slave server)
	//args：none
	//author: lizezheng
	//date: 2021/05/26
	std::optional<string> slave(context&& ctx) noexcept
	{
		auto server = SonServer::getSonServer();
		if (server != nullptr && server->getCas() == SonServer::serverCas::sync)//确实存在子服务器
		{
			return code::getSingleReply(code::endpoint_toString(server->getAddress()));
		}
		else return code::no_slave_error;
	}
	//master ( reply the ip and port of slave server)
	//args：none
	//author: lizezheng
	//date: 2021/05/26
	std::optional<string> master(context&& ctx) noexcept
	{
		auto server = FatherServer::getFatherServer();
		if (server != nullptr && server->getCas() == FatherServer::serverCas::sync)//确实存在主服务器
		{
			return code::getSingleReply(code::endpoint_toString(server->getAddress()));
		}
		else return code::no_master_error;
	}
}
