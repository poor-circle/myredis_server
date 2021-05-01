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
			//ע�⣬��ֻ��һ��DEMO,��Ȼ�㲥�����еĿͻ��ˣ����������ͻ�����Ҫ���������һ��������������鿴���

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
}