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
	std::optional<string> test(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			auto myID=ctx.session.getSessionID();//获取本会话的id

			auto& table = BaseSession::getSessionMap();//获取记录了所有当前会话id的hash表
			for (auto& e : table)//遍历所有当前的会话
			{
				auto session = e.second;
				auto ID = e.first;
				if (ID != myID)//如果不是本会话
				{
					auto str = code::getBulkReply(fmt::format("your ID is {}", ID));
					//新建一个协程，在这个协程上给对应的客户端发送消息
					session->addNewCoroToSendMessage(std::move(str));
				}
			}

			return code::getIntegerReply(table.size()-1);
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}
	//一个测试函数，模拟等待一条订阅消息，无参数
	std::optional<string> wait(context&& ctx) noexcept
	{
		//返回空字符串，则redis客户端会等待消息
		return "";
	}

	/*
	* subscribe channel [channel...]
	* @author:tigerwang
	* date:2021/5/3
	*/
	std::optional<string> subscribe(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{	
			if (args.size() < 2) {
				return code::args_count_error;
			}
			return code::getMultiReply(args.begin() + 1, args.end(),
				[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
			{	
				auto myID = ctx.session.getSessionID();//获取本会话的id
				// 将订阅的频道记录到订阅表里
				auto& subChannels = ctx.session.getsubChannels();
				subChannels->insert(*arg);

				auto& channelTable = ctx.session.getChannelMap();
				auto channelIter = channelTable.find(*arg);
				if (channelIter == channelTable.end())
				{
					// 没找到新建一个频道
					hash_set<size_t> subList;
					subList.emplace(myID);
					channelTable.insert(pair<string, hash_set<size_t>>(*arg, subList));

				}
				else
				{
					// 将id号挂到对应的频道列表后
					channelIter->second.insert(myID);
				}

				code::getBulkReplyTo("subscribe", s);
				code::getBulkReplyTo(*arg, s);
				// 返回订阅的频道数
				code::getIntegerReplyTo(subChannels->size(), s);
				return 3;
			});
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* unsubscribe channel [channel...]
	* @author:tigerwang
	* date:2021/5/4
	*/
	std::optional<string> unsubscribe(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			// 无参数，退订所有频道 
			if(args.size()==1)
			{
				auto& subChannels = ctx.session.getsubChannels();

				std::vector<string> channels;
				for (auto iter = subChannels->begin(); iter != subChannels->end(); iter++) {
					channels.emplace_back(*iter);
				}
				return code::getMultiReply(channels.begin(), channels.end(),
					[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{
					auto myID = ctx.session.getSessionID();//获取本会话的id
					// 在订阅列表中删去订阅频道	
					auto& subChannels = ctx.session.getsubChannels();
					subChannels->erase(*arg);

					// 全局订阅表
					auto& channelTable = ctx.session.getChannelMap();
					auto channelIter = channelTable.find(*arg);
					if (channelIter != channelTable.end()) {
						channelIter->second.erase(myID);
						if (channelIter->second.empty()) {
							// 如果该频道没有订阅者就删除该频道
							channelTable.erase(channelIter);
						}
					}

					code::getBulkReplyTo("unsubscribe", s);
					code::getBulkReplyTo(*arg, s);
					// 返回订阅的频道数
					code::getIntegerReplyTo(subChannels->size(), s);
					return 3;
				});
			}

			return code::getMultiReply(args.begin() + 1, args.end(),
				[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
			{
				auto myID = ctx.session.getSessionID();//获取本会话的id
				// 在订阅列表中删去订阅频道	
				auto& subChannels = ctx.session.getsubChannels();
				subChannels->erase(*arg);

				// 全局订阅表
				auto& channelTable = ctx.session.getChannelMap();
				auto channelIter = channelTable.find(*arg);
				if (channelIter != channelTable.end()) {
					channelIter->second.erase(myID);
					if (channelIter->second.empty()) {
						// 如果该频道没有订阅者就删除该频道
						channelTable.erase(channelIter);
					}
				}
				
				code::getBulkReplyTo("unsubscribe", s);
				code::getBulkReplyTo(*arg, s);
				// 返回订阅的频道数
				code::getIntegerReplyTo(subChannels->size(), s);
				return 3;
			});
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* pubsub subcommand [argument [argument ...]]
	* subcommand:
	* pubsub channels [pattern]		
	* pubsub numsub	[channels...] 指定信道的订阅者个数
	* pubsub numpat 客户端订阅的所有模式的总和
	* @author:tigerwang
	* date:2021/5/4
	*/
	std::optional<string> pubsub(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 2) {
				return code::args_count_error;
			}
			auto subcommand = args[1];
			boost::algorithm::to_lower(subcommand);
			if (subcommand == "numsub") {
				// 返回指定信道的订阅者个数
				if (args.size() < 3) {
					return code::args_count_error;
				}
				return code::getMultiReply(args.begin() + 2, args.end(),
					[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{

					auto myID = ctx.session.getSessionID();//获取本会话的id

					auto& channelTable = ctx.session.getChannelMap();
					auto channelIter = channelTable.find(*arg);
					if (channelIter == channelTable.end())
					{
						code::getBulkReplyTo(*arg, s);
						// 返回订阅的频道数
						code::getIntegerReplyTo(0, s);
					}
					else
					{
						code::getBulkReplyTo(*arg, s);
						// 返回订阅的频道数
						code::getIntegerReplyTo(channelIter->second.size(), s);
					}


					return 2;
				});
			}
			else {
				return code::command_error;
			}
			
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}
}