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
			string s;
			for (auto arg=args.begin()+1;arg!=args.end();++arg)
			{
				auto myID = ctx.session.getSessionID();//获取本会话的id
				// 将订阅的频道记录到订阅表里
				auto& subChannels = ctx.session.getsubChannels();
				subChannels.emplace(*arg);
				BaseSession::getChannelMap()[*arg].emplace(myID);
				code::getMultiReplyTo(back_inserter(s),"subscribe", *arg, subChannels.size());// 返回订阅成功的信息
			}
			return s;
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
		auto& subChannels = ctx.session.getsubChannels();
		auto& channelMap = BaseSession::getChannelMap();
		try
		{
			string ret;
			// 无参数，退订所有频道 
			if(args.size()==1)
			{
				while (!subChannels.empty())
				{
					//获取一条多播回复(一共三条内容）
					code::getMultiReplyTo(back_inserter(ret),"unsubscribe",*subChannels.begin(), subChannels.size()-1);
					auto iter = channelMap.find(*subChannels.begin());
					auto channels = iter->second;
					channels.erase(ctx.session.getSessionID());
					if (channels.empty()) channelMap.erase(iter);//如果频道已经没有人订阅就删掉
					subChannels.erase(subChannels.begin());
				}
			}
			else
			{
				for (auto channel = args.begin() + 1; channel != args.end(); ++channel)//遍历所有传入的频道名
				{
					auto subChannelIter = subChannels.find(*channel);
					if (subChannelIter != subChannels.end())//如果本会话订阅了这个频道
					{
						
						auto iter = channelMap.find(*channel);
						auto channels = iter->second;
						channels.erase(ctx.session.getSessionID());
						if (channels.empty()) channelMap.erase(iter);//如果频道已经没有人订阅就删掉
						subChannels.erase(subChannelIter);
					}
					//获取一条多播回复(一共三条内容）
					code::getMultiReplyTo(back_inserter(ret), "unsubscribe", *channel, subChannels.size());
				}
			}
			return ret;
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
			if (subcommand == "numsub"sv) {
				// 返回指定信道的订阅者个数
				if (args.size() < 3) {
					return code::args_count_error;
				}
				return code::getMultiReplyByRange(args.begin() + 2, args.end(),
					[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{
					auto& channelTable = ctx.session.getChannelMap();
					auto channelIter = channelTable.find(*arg);
					code::getBulkReplyTo(*arg, s);
					if (channelIter == channelTable.end())
						code::getIntegerReplyTo(0, s);// 返回订阅的频道数
					else
						code::getIntegerReplyTo(channelIter->second.size(), s);// 返回订阅的频道数
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

	/*
	* publish channel message
	* @author:tigerwang
	* date:2021/5/5
	* TODO:发送的时候也要遍历模式一起发 过期ID的pattern要删掉
	*/
	std::optional<string> publish(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		auto& channelMap = BaseSession::getChannelMap();//全局订阅表
		auto& table = BaseSession::getSessionMap();//获取记录了所有当前会话id的hash表
		auto& patternTable = BaseSession::getPatternTable();
		auto& subPatterns = ctx.session.getsubPatterns();
		try
		{
			if (args.size() != 3) {
				return code::args_count_error;
			}
			auto iter = channelMap.find(args[1]);
			int64_t pubCnt = 0;
			if (iter != channelMap.end())
			{
				auto sessionIDSet = iter->second;//获取订阅该频道的sessionID 一个hash_set
				pubCnt = sessionIDSet.size();
				for (auto& e : sessionIDSet)
				{
					auto session = table.find(e)->second;
					session->addNewCoroToSendMessage(code::getMultiReply("message", iter->first, args[2]));
				}
			}

			// 遍历模式表，向订阅匹配模式的ID发送信息
			for (auto iter = patternTable.begin(); iter != patternTable.end(); iter++)
			{
				Pattern p = iter->first;
				// 频道和regex匹配,向该regex下的所有ID发送
				if (std::regex_match(args[1].c_str(), p.getRegex()))
				{
					pubCnt += iter->second.size();
					for (auto& e : iter->second) {
						auto session = table.find(e)->second;
						session->addNewCoroToSendMessage(code::getMultiReply("message", iter->first, args[2]));
					}
				}
			}

			return code::getIntegerReply(pubCnt);
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* psubscribe pattern [pattern...]
	* @author:tigerwang
	* date:2021/5/5
	*/
	std::optional<string> psubscribe(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		auto& channelMap = BaseSession::getChannelMap();
		auto& subChannels = ctx.session.getsubChannels();
		auto& patternTable = BaseSession::getPatternTable();
		auto& subPatterns = ctx.session.getsubPatterns();
		auto myID = ctx.session.getSessionID();//获取本会话的id
		try
		{
			if (args.size() < 2) {
				return code::args_count_error;
			}
			string s;
			for (auto arg = args.begin() + 1; arg != args.end(); ++arg)
			{
				// 构造正则表达式
				std::regex rx;
				auto flag = regex_constants::ECMAScript;
				if (objectMap.size() >= regexOpLowerBound)
					flag |= regex_constants::syntax_option_type::optimize;
				rx = regex((*arg).c_str(), flag);
				Pattern p = { std::move(*arg),std::move(rx) };
				// 全局模式表
				patternTable[p].emplace(myID);
				// 在订阅模式中增加一个模式
				subPatterns.emplace(p);
				
				code::getMultiReplyTo(back_inserter(s), "psubscribe", p.getPatternStr(), subPatterns.size());// 返回订阅成功的信息
			}
			return s;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}
}