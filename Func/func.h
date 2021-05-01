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

	using syncFuncPtr = std::optional<boost::container::string>(*)(context&& ctx) noexcept;//同步api的函数指针
	using asyncFuncPtr = asio::awaitable<std::optional<boost::container::string>>(*)(context&& ctx) noexcept;//异步api的函数指针

	enum class funcType
	{
		read,
		write,
		connect,
		blocked,
		pubsub,
		async_pubsub,
	};
	union funcPtr
	{
		syncFuncPtr syncFunc;//同步函数指针
		asyncFuncPtr asyncFunc;//异步函数指针
	};
	struct funcInfo
	{
		syncFuncPtr syncptr;
		asyncFuncPtr asyncptr;//这里之所以不做成union类型，是因为其似乎无法处理协程函数？奇怪
		funcType type;//函数类型
		bool isAsyncFunc(void) const noexcept
		{
			return type == funcType::async_pubsub;
		}
		funcInfo(syncFuncPtr ptr, funcType type):syncptr(ptr),asyncptr(nullptr),type(type){}
		funcInfo(asyncFuncPtr ptr, funcType type) :syncptr(nullptr), asyncptr(ptr), type(type) {}
	};


	//命令层类似于spring boot的controller层，每一个函数对应一个myredis命令
	//绝大部分情况下不需要关心网络层的细节
}