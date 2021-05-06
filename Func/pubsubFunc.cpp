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
				code::getMultiReplyTo(back_inserter(s),"subscribe", *arg, (int64_t)subChannels.size());// 返回订阅成功的信息
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
					code::getMultiReplyTo(back_inserter(ret),"unsubscribe",*subChannels.begin(), (int64_t)subChannels.size()-1);
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
					code::getMultiReplyTo(back_inserter(ret), "unsubscribe", *channel, (int64_t)subChannels.size());
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
						session->addNewCoroToSendMessage(code::getMultiReply("message", iter->first.getPatternStr(),args[1], args[2]));
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
				
				code::getMultiReplyTo(back_inserter(s), "psubscribe", p.getPatternStr(), (int64_t)subPatterns.size());// 返回订阅成功的信息
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
	* punsubscribe pattern [pattern...]
	* @author:tigerwang
	* date:2021/5/6
	*/
	std::optional<string> punsubscribe(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();

		auto& patternTable = BaseSession::getPatternTable();
		auto& subPatterns = ctx.session.getsubPatterns();
		try
		{
			string ret;
			// 无参数，退订所有频道 
			if (args.size() == 1)
			{
				while (!subPatterns.empty())
				{
					//获取一条多播回复(一共三条内容）
					code::getMultiReplyTo(back_inserter(ret), "unsubscribe", subPatterns.begin()->getPatternStr(), (int64_t)subPatterns.size() - 1);
					auto iter = patternTable.find(*subPatterns.begin());
					auto patterns = iter->second;
					patterns.erase(ctx.session.getSessionID());
					if (patterns.empty()) patternTable.erase(iter);//如果频道已经没有人订阅就删掉
					subPatterns.erase(subPatterns.begin());
				}
			}
			else
			{
				for (auto patternIter = args.begin() + 1; patternIter != args.end(); ++patternIter)//遍历所有传入的模式名
				{
					std::regex rx;
					string str = *patternIter;
					Pattern temp = { std::move(str),std::move(rx) };
					auto subPatternIter = subPatterns.find(temp);
					if (subPatternIter != subPatterns.end())//如果本会话订阅了这个频道
					{
						// 去全局模式表中删除会话ID
						auto iter = patternTable.find(temp);
						auto channels = iter->second;
						channels.erase(ctx.session.getSessionID());
						if (channels.empty()) patternTable.erase(iter);//如果频道已经没有人订阅就删掉
						// 删除订阅模式表中的对应模式
						subPatterns.erase(subPatternIter);
					}
					//获取一条多播回复(一共三条内容）
					code::getMultiReplyTo(back_inserter(ret), "unsubscribe", *patternIter, (int64_t)subPatterns.size());
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
}