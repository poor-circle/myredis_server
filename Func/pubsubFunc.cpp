#include "stdafx.h"
#include "func.h"
#include "pubsubFunc.h"
#include "../object.hpp"
#include "../code.h"
namespace myredis::func
{
	
#include"../namespace.i"

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
				auto myID = ctx.session.getSessionID();//��ȡ���Ự��id
				// �����ĵ�Ƶ����¼�����ı���
				auto& subChannels = ctx.session.getsubChannels();
				subChannels.emplace(*arg);
				BaseSession::getChannelMap()[*arg].emplace(myID);
				code::getMultiReplyTo(back_inserter(s),"subscribe", *arg,ctx.session.getSubscribeCount());// ���ض��ĳɹ�����Ϣ
			}
			return s;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
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
			// �޲������˶�����Ƶ�� 
			if(args.size()==1)
			{
				while (!subChannels.empty())
				{
					//��ȡһ���ಥ�ظ�(һ���������ݣ�
					code::getMultiReplyTo(back_inserter(ret),"unsubscribe",*subChannels.begin(), ctx.session.getSubscribeCount() -1);
					auto iter = channelMap.find(*subChannels.begin());
					auto channels = iter->second;
					channels.erase(ctx.session.getSessionID());
					if (channels.empty()) channelMap.erase(iter);//���Ƶ���Ѿ�û���˶��ľ�ɾ��


					subChannels.erase(subChannels.begin());
				}
			}
			else
			{
				for (auto channel = args.begin() + 1; channel != args.end(); ++channel)//�������д����Ƶ����
				{
					auto subChannelIter = subChannels.find(*channel);
					if (subChannelIter != subChannels.end())//������Ự���������Ƶ��
					{
						
						auto iter = channelMap.find(*channel);
						auto channels = iter->second;
						channels.erase(ctx.session.getSessionID());
						if (channels.empty()) channelMap.erase(iter);//���Ƶ���Ѿ�û���˶��ľ�ɾ��
						subChannels.erase(subChannelIter);
					}
					//��ȡһ���ಥ�ظ�(һ���������ݣ�
					code::getMultiReplyTo(back_inserter(ret), "unsubscribe", *channel, ctx.session.getSubscribeCount());
				}
			}
			return ret;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* pubsub subcommand [argument [argument ...]]
	* subcommand:
	* pubsub channels [pattern]		
	* pubsub numsub	[channels...] ָ���ŵ��Ķ����߸���
	* pubsub numpat �ͻ��˶��ĵ�����ģʽ���ܺ�
	* @author:tigerwang
	* date:2021/5/4
	*/
	std::optional<string> pubsub(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		auto& channelTable = BaseSession::getChannelMap();
		auto& subChannels = ctx.session.getsubChannels();
		auto& patternTable = BaseSession::getPatternTable();
		auto& subPatterns = ctx.session.getsubPatterns();
		try
		{
			if (args.size() < 2) {
				return code::args_count_error;
			}
			auto subcommand = args[1];
			boost::algorithm::to_lower(subcommand);
			if (subcommand == "numsub"sv) {
				// ����ָ���ŵ��Ķ����߸���
				if (args.size() < 3) {
					return code::args_count_error;
				}
				return code::getMultiReplyByRange(args.begin() + 2, args.end(),
					[&channelTable](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{
					auto channelIter = channelTable.find(*arg);
					code::getBulkReplyTo(*arg, s);
					if (channelIter == channelTable.end())
						code::getIntegerReplyTo(0, s);// ���ض��ĵ�Ƶ����
					else
						code::getIntegerReplyTo(channelIter->second.size(), s);// ���ض��ĵ�Ƶ����
					return 2;
				});
			}
			else if (subcommand == "channels"sv) {
				if (args.size() == 2)
				{
					// �޲��� �г����л�Ծ�ŵ�(����������1)
					return code::getMultiReplyByRange(channelTable.begin(), channelTable.end(),
						[](hash_map<string,hash_set<size_t>>::iterator arg, std::back_insert_iterator<string> s)
					{
						code::getBulkReplyTo(arg->first,s);
						return 1;
					});
					
				}
				else if(args.size()==3)
				{
					// ����ƥ��ģʽ��Ƶ��
					return code::getMultiReplyByRange(args.begin()+2, args.end(),
						[&channelTable](vector<string>::iterator arg, std::back_insert_iterator<string> s)
					{
						int64_t cnt = 0;
						std::regex rx;
						auto flag = regex_constants::ECMAScript;
						if (channelTable.size() >= regexOpLowerBound)
							flag |= regex_constants::syntax_option_type::optimize;
						rx = regex((*arg).c_str(), flag);
						for (auto& e : channelTable) {
							if (std::regex_match(e.first.c_str(), rx)) {
								cnt++;
								code::getBulkReplyTo(e.first, s);
							}
						}
						return cnt;
					});
				}
				else {
					return code::args_count_error;
				}
			}
			else if (subcommand == "numpat"sv)
			{
				// ����ģʽ���пͻ��˵�����
				int64_t cnt = 0;
				for (auto& e : patternTable)
				{
					cnt += e.second.size();
				}
				return code::getIntegerReply(cnt);
			}
			else {
				return code::command_error;
			}
			
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
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
		auto& channelMap = BaseSession::getChannelMap();//ȫ�ֶ��ı�
		auto& table = BaseSession::getSessionMap();//��ȡ��¼�����е�ǰ�Ựid��hash��
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
				auto sessionIDSet = iter->second;//��ȡ���ĸ�Ƶ����sessionID һ��hash_set
				pubCnt = sessionIDSet.size();
				for (auto& e : sessionIDSet)
				{
					auto session = table.find(e)->second;
					session->addNewCoroToSendMessage(code::getMultiReply("message", iter->first, args[2]));
				}
			}

			// ����ģʽ������ƥ��ģʽ��ID������Ϣ
			for (auto iter = patternTable.begin(); iter != patternTable.end(); iter++)
			{
				Pattern p = iter->first;
				// Ƶ����regexƥ��,���regex�µ�����ID����
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
			return nullopt;//���ؿ�ֵ
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
		auto myID = ctx.session.getSessionID();//��ȡ���Ự��id
		try
		{
			if (args.size() < 2) {
				return code::args_count_error;
			}
			string s;
			for (auto arg = args.begin() + 1; arg != args.end(); ++arg)
			{
				// ����������ʽ
				std::regex rx;
				auto flag = regex_constants::ECMAScript;
				if (patternTable.size() >= regexOpLowerBound)
					flag |= regex_constants::syntax_option_type::optimize;
				rx = regex((*arg).c_str(), flag);
				Pattern p = { std::move(*arg),std::move(rx) };
				// ȫ��ģʽ��
				patternTable[p].emplace(myID);
				// �ڶ���ģʽ������һ��ģʽ
				subPatterns.emplace(p);
				
				code::getMultiReplyTo(back_inserter(s), "psubscribe", p.getPatternStr(), ctx.session.getSubscribeCount());// ���ض��ĳɹ�����Ϣ
			}
			return s;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
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
		auto&& sessionID = ctx.session.getSessionID();
		auto& patternTable = BaseSession::getPatternTable();
		auto& subPatterns = ctx.session.getsubPatterns();
		try
		{
			string ret;
			// �޲������˶�����Ƶ�� 
			if (args.size() == 1)
			{
				while (!subPatterns.empty())
				{
					//��ȡһ���ಥ�ظ�(һ���������ݣ�
					code::getMultiReplyTo(back_inserter(ret), "punsubscribe", subPatterns.begin()->getPatternStr(), ctx.session.getSubscribeCount() - 1);
					auto iter = patternTable.find(*subPatterns.begin());
					auto& patterns = iter->second;
					patterns.erase(ctx.session.getSessionID());
					if (patterns.empty()) patternTable.erase(iter);//���Ƶ���Ѿ�û���˶��ľ�ɾ��
					subPatterns.erase(subPatterns.begin());
				}
			}
			else
			{
				for (auto patternIter = args.begin() + 1; patternIter != args.end(); ++patternIter)//�������д����ģʽ��
				{
					std::regex rx;
					string str = *patternIter;
					Pattern temp = { std::move(str),std::move(rx) };
					auto subPatternIter = subPatterns.find(temp);
					if (subPatternIter != subPatterns.end())//������Ự���������Ƶ��
					{
						// ȥȫ��ģʽ����ɾ���ỰID
						auto iter = patternTable.find(temp);
						if (iter != patternTable.end()) {
							auto& channels = iter->second;
							channels.erase(sessionID);
							if (channels.empty()) patternTable.erase(iter);//���Ƶ���Ѿ�û���˶��ľ�ɾ��

							// ɾ������ģʽ���еĶ�Ӧģʽ
							subPatterns.erase(subPatternIter);
						}

					}
					//��ȡһ���ಥ�ظ�(һ���������ݣ�
					code::getMultiReplyTo(back_inserter(ret), "punsubscribe", *patternIter, (int64_t)subPatterns.size());
				}
			}
			return ret;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}


	bool isAllowWhenSubscribed(const string& cmd)
	{
		static hash_set <string> allowList= {"subscribe","unsubscribe","psubscribe","punsubscribe","ping","quit"};
		return allowList.contains(cmd);
	}


}