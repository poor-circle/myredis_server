#pragma once
#include"../stdafx.h"
#include"../BaseSession.h"
namespace myredis::func
{
	struct Ctx
	{
		std::vector<string>&& args;
		BaseSession& session;
		Ctx(std::vector<string>&& args, BaseSession& session) :
			args(std::move(args)), session(session) {}
	};

	//myredis命令类型：
	//返回值：string，用于返回给客户端
	//参数：func::Ctx
	//保证所有异常都在内部捕获，不抛出任何异常
	
	using funcPtr = std::optional<boost::container::string>(*)(Ctx&&) noexcept;

	//命令层类似于spring boot的controller层，每一个函数对应一个myredis命令
	//绝大部分情况下不需要关心网络层的细节

	
}