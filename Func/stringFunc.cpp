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
			auto iter = getObjectMap().find(std::move(args[1]));
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				return code::key_search_error;
			}
			else
			{
				auto& ret = visit([](auto& e) {return visitor::get(e); }, iter->second).second;
				return code::getBulkReply(ret);
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
				// ����set����
				getObjectMap().update(std::move(args[1]), stringToObject(std::move(args[2])));
				return code::succeed;
			}
			else
			{
				// �ҵ�keyȡ��
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
			
				if (ret.first != code::code::success) {
					return code::getErrorReply(std::move(ret.second));
				}
				// append����
				string& appendString = ret.second.append(std::move(args[2]));
				INT64 len = appendString.size();
				// ����
				iter->second = stringToObject(std::move(appendString));
				return code::getIntegerReply(std::move(len));
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
				//�Ҳ�����Ӧ��key ���ش���
				return code::key_search_error;
			}
			else
			{
				// �ҵ�keyȡ��
				auto& ret = visit([](auto& e) {return visitor::get(e); }, iter->second).second;
				INT64 len = ret.size();
				return code::getIntegerReply(std::move(len));
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