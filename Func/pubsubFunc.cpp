#include "stdafx.h"
#include "func.h"
#include "pubsubFunc.h"
#include "../object.hpp"
#include "../code.h"
namespace myredis::func
{
	
#include"../namespace.i"
	// test	created by lizezheng	date:2021/5/01
	//一个测试函数，向其他所有客户端广播一条消息（事先输入wait才能正确同步显示），无参数
	asio::awaitable<std::optional<string>> test(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			//注意，这个函数是异步函数，类型不同于之前的同步函数
			//同步函数在整个调用过程内都是原子的，异步函数在co_await之间是原子的（co_await会主动让出cpu）
			//异步函数支持直接使用网络io来发送消息
			
			auto myID=ctx.session.getSessionID();//获取本会话的id

			auto& table = BaseSession::getSessionMap();//获取记录了所有当前会话id的hash表
			for (auto& e : table)
			{
				auto session = e.second;
				auto ID = e.first;
				if (ID != myID)
				{
					auto str = code::getBulkReply(fmt::format("your ID is {}", ID));
					co_await asio::async_write(session->getSocket(), asio::buffer(str.c_str(),str.size()), use_awaitable);
				}
			}
			//注意，这只是一个DEMO,虽然广播给所有的客户端，但是其他客户端需要随便乱输入一条错误的命令来查看结果

			co_return code::getIntegerReply(table.size()-1);
		}
		catch (const exception& e)
		{
			printlog(e);
			co_return nullopt;//返回空值
		}
	}
	//一个测试函数，模拟等待一条订阅消息，无参数
	std::optional<string> wait(context&& ctx) noexcept
	{
		//返回空字符串，则redis客户端会等待消息
		return "";
	}
}