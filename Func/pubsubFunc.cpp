#include "stdafx.h"
#include "func.h"
#include "pubsubFunc.h"
#include "../object.hpp"
#include "../code.h"
namespace myredis::func
{
	
#include"../namespace.i"

	 
	// test	created by lizezheng	date:2021/5/01
	//һ�����Ժ��������������пͻ��˹㲥һ����Ϣ����������wait������ȷͬ����ʾ�����޲���
	std::optional<string> test(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			auto myID=ctx.session.getSessionID();//��ȡ���Ự��id

			auto& table = BaseSession::getSessionMap();//��ȡ��¼�����е�ǰ�Ựid��hash��
			for (auto& e : table)//�������е�ǰ�ĻỰ
			{
				auto session = e.second;
				auto ID = e.first;
				if (ID != myID)//������Ǳ��Ự
				{
					auto str = code::getBulkReply(fmt::format("your ID is {}", ID));
					//�½�һ��Э�̣������Э���ϸ���Ӧ�Ŀͻ��˷�����Ϣ
					session->addNewCoroToSendMessage(std::move(str));
				}
			}

			return code::getIntegerReply(table.size()-1);
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	//һ�����Ժ�����ģ��ȴ�һ��������Ϣ���޲���
	std::optional<string> wait(context&& ctx) noexcept
	{
		//���ؿ��ַ�������redis�ͻ��˻�ȴ���Ϣ
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
				auto myID = ctx.session.getSessionID();//��ȡ���Ự��id
				// �����ĵ�Ƶ����¼�����ı���
				auto& subChannels = ctx.session.getsubChannels();
				subChannels->insert(*arg);

				auto channelTable = ctx.session.getChannelMap();
				auto channelIter = channelTable.find(*arg);
				if (channelIter == channelTable.end())
				{
					// û�ҵ��½�һ��Ƶ��
					hash_set<size_t> subList;
					subList.emplace(myID);
					channelTable.emplace(pair<string, hash_set<size_t>>(*arg, subList));

				}
				else
				{
					// ��id�Źҵ���Ӧ��Ƶ���б��
					channelIter->second.insert(myID);
				}

				code::getBulkReplyTo("subscribe", s);
				code::getBulkReplyTo(*arg, s);
				// ���ض��ĵ�Ƶ����
				code::getIntegerReplyTo(subChannels->size(), s);
				return 3;
			});
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
}