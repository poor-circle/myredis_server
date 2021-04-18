#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/StringVisitor/get.h"

//func���൱��spring boot��controller��
//ͨ��controller����visitor��
//���ø�ʽ��visit([](auto& e) {return visitor::funcName(e); }, object)
//visitor::funcName����Ҫ���õĺ�����
//object����Ҫ�����Ķ�������
namespace myredis::func
{
#include"../namespace.i"

	//����һ���ַ���

	//��������ÿһ������Ӧ�þ����ܱ�֤����������

	//noexcept:ÿһ����������֤�����׳��κ��쳣����ʹ��һ������������Ҳ�ܱ�֤��Ӱ����������

	optional<string> set(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			getObjectMap().update(std::move(args[1]), stringToObject(std::move(args[2])));
			return code::succeed;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	//��ȡһ���ַ���
	optional<string> get(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				return code::key_search_error;
			}
			else
			{
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.second);
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}",e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	//append  
	/*
	* append ���string����
	* ��� key �Ѿ����ڣ�����ֵΪ�ַ�������ô��������� value ׷�ӵ�ԭ��ֵ��value���Ľ�β��������string�ĳ��ȡ� 
	* ��� key �����ڣ���ô�������ȴ���һ�����ַ�����key����ִ��׷�Ӳ������������ APPEND �������� SET ������
	* code by tigerwang  2021/4/17 20:00
	*/
	std::optional<string> append(std::vector<string>&& args) noexcept 
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				auto len = args[2].size();
				// ����set����
				getObjectMap().update(std::move(args[1]), stringToObject(std::move(args[2])));
				//set�Ժ�Ҫ���س���
				return code::getIntegerReply(len);
			}
			else
			{
				// �ҵ�keyȡ�� 
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
			
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.second);
				}
				// append����
				ret.second.append(args[2]);
				int64_t len = ret.second.size();
				iter->second = stringToObject(std::move(ret.second));
				return code::getIntegerReply(len);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	// strlen
	/* strlen ���string���͵Ķ���
	* ����key��string����value�ĳ��ȡ�
	* ���key��Ӧ�ķ�string���ͣ��ͷ��ش���
	* ���key�����ڷ���0
	* code by tigerwang  2021/4/18 10:00
	*/
	std::optional<string> strlen(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())
			{
				//�Ҳ�����Ӧ��key ����0
				return code::getIntegerReply(0);
			}
			else
			{
				// �ҵ�keyȡ��
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.second);
				}
				return code::getIntegerReply(ret.second.size());
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}


	std::optional<string> ping(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() == 1)
				return code::pong;
			else if (args.size() == 2)
				return code::getSingleReply(args[1]);
			return code::args_count_error;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
}