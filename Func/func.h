#pragma once
#include"../stdafx.h"
#include "../BaseSession.h"
namespace myredis::func
{
	//myredis命令类型：
	//返回值：string，用于返回给客户端
	//参数：一个引用类型，指向一个vector<string>
	//保证所有异常都在内部捕获，不抛出任何异常
	
	struct context//连接上下文
	{
		std::vector<string>&& args;//本次命令传递的参数
		BaseSession& session;//连接的引用
		context(std::vector<string>&& args,BaseSession& session):
			args(std::move(args)),session(session){}
	};

	using funcPtr = std::optional<boost::container::string>(*)(context&& ctx) noexcept;

	enum funcType
	{
		read,
		write,
		connect
	};
	struct funcInfo
	{
		funcPtr func;//函数指针
		funcType type;//函数类型
	};


	//命令层类似于spring boot的controller层，每一个函数对应一个myredis命令
	//绝大部分情况下不需要关心网络层的细节
}