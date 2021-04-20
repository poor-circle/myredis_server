#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/StringVisitor/get.h"
#include "../ObjectVisitor/StringVisitor/changeCodetoString.h"
#include "../ObjectVisitor/StringVisitor/incr.h"
#include "../ObjectVisitor/StringVisitor/decr.h"


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

	optional<string> set(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			objectMap.update(std::move(args[1]), stringToObject(std::move(args[2])));
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	//��ȡһ���ַ���
	optional<string> get(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())//�Ҳ�����Ӧ��key
			{
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
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

	/*
	* append
	* @author:tigerwang
	* date:2021/4/18
	*/
	std::optional<string> append(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())//�Ҳ�����Ӧ��key
			{
				auto len = args[2].size();
				// ����set����
				objectMap.update(std::move(args[1]), stringToObject(std::move(args[2])));
				//set�Ժ�Ҫ���س���
				return code::getIntegerReply(len);
			}
			else
			{
				//���������ǿ��ת��Ϊ�ַ���
				auto ret = visit([iter](auto& e)
				{
					return visitor::changeCodetoString(e,iter->second);
				}, iter->second);
			
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}
				// append����
				ret.second.append(args[2]);
				return code::getIntegerReply(ret.second.size());
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/* 
	* strlen
	* @author:tigerwang
	* date:2021/4/18
	*/
	std::optional<string> strlen(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				//�Ҳ�����Ӧ��key ����0
				return code::getIntegerReply(0);
			}
			else
			{
				// �ҵ�keyȡ��
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
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

	/*
	* usage: GETRANGE key start end
	* @author:tigerwang
	* date:2021/4/18
	*/
	std::optional<string> getrange(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				//�Ҳ�����Ӧ��key
				return code::nil;
			}
			else
			{
				// �ҵ�keyȡ��string
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}
				// ʹ��stoi��start��endת��ΪInt
				// ����ת�����׳�invalid_argument�쳣
				// �������ػ��׳�out_of_range�쳣
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false || 
					try_lexical_convert(args[3], end) == false) {
					return code::getErrorReply(code::status::invaild_argument);
				}
				const int64_t strLen = ret.second.size();
				// ���С���� + strLen ����С��0˵�����������ˣ�ֱ����0
				if (start < 0) 
					start = strLen + start;
				if (start < 0) 
					start = 0;
				if (end < 0)
					end = strLen + end;
				if (end < 0)
					end = 0;
				
				int64_t len = end - start + 1;
				if (start + len > strLen) 
				{
					len = strLen - start;
				}
				if (start > strLen || len < 0)
				{
					return code::getBulkReply("");
				}
				return code::getBulkReply(string_view(ret.second.c_str()+ start,len));
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* setnx 
	* @author:tigerwang
	* date:2021/4/18
	*/
	std::optional<string> setnx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter= objectMap.try_insert(std::move(args[1]), stringToObject(std::move(args[2])));
			return code::getIntegerReply(iter != objectMap.end()?1:0);
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* getset 
	* @author:tigerwang
	* date:2021/4/18
	*/
	std::optional<string> getset(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())//�Ҳ�����Ӧ��key
			{
				// ����nil
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				// ��ȡ֮ǰ��ֵ
				auto retstr = code::getBulkReply(ret.second);

				// ����
				iter->second = stringToObject(std::move(args[2]));

				// ����֮ǰ��ֵ
				return retstr;
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
	
	/*
	* msetnx
	* @author: tigerwang 
	* date:2021/4/19
	*/
	std::optional<string> msetnx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() > 1 && args.size() % 2 == 0)
				return code::args_count_error;
			for (auto arg = args.begin() + 1; arg < args.end(); arg += 2) 
			{
				auto iter = objectMap.find(*arg);
				if (iter != objectMap.end())
				{
					return code::getIntegerReply(0);
				}
			}
			for (auto arg = args.begin() + 1; arg < args.end(); arg += 2)
			{
				objectMap.update(std::move(*arg), stringToObject(std::move(*(arg + 1))));
			}
			return code::getIntegerReply(1);
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* incr
	* @author: tigerewang
	* date:2021/4/19
	*/
	std::optional<string> incr(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) {
				objectMap.update(std::move(args[1]), (int64_t)1);
				return code::getIntegerReply(1);
			}
			else {
				// ��visit��+1
				auto ret = visit([](auto& e)
				{
					return visitor::incr(e);
				}, iter->second);

				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);
			}
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* incrby
	* @author: tigerwang 
	* date:2021/4/20
	* 
	*/
	std::optional<string> incrby(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			int64_t increment;
			if (!try_lexical_convert<int64_t>(args[2], increment)) {
				return code::getErrorReply(code::status::invaild_argument);
			}

			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) {
				objectMap.update(std::move(args[1]), increment);
				return code::getIntegerReply(increment);
			}
			else {
				// ��visit��+increment
				auto ret = visit([increment](auto& e )
				{
					return visitor::incrby(e,increment);
				}, iter->second);

				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);
			}
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}

	}

	/*
	* incrbyfloat
	* @author: tigerwang
	* date:2021/4/20
	* TODO: ��int64_t+float�Ĵ��� 
	*/
	std::optional<string> incrbyfloat(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			double increment;
			if (!try_lexical_convert<double>(args[2], increment)) {
				return code::getErrorReply(code::status::invaild_argument);
			}

			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) {
				objectMap.update(std::move(args[1]), increment);
				return code::getBulkReply(to_string(increment));
			}
			else {

				auto ret = visit([iter,increment](auto& e)
				{
					return visitor::incrbyfloat(e, iter->second,increment);
				}, iter->second);


				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}

				return code::getBulkReply(ret.second);
			}
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}

	}

	/*
	* decr
	* @author: tigerewang
	* date:2021/4/19
	*/
	std::optional<string> decr(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) {
				objectMap.update(std::move(args[1]), (int64_t)-1);
				return code::getIntegerReply(-1);
			}
			else {
				// ��visit��+1
				auto ret = visit([](auto& e)
				{
					return visitor::decr(e);
				}, iter->second);

				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);
			}
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* decrby
	* @author: tigerwang
	* date:2021/4/20
	*/
	std::optional<string> decrby(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			int64_t decrement;
			if (!try_lexical_convert<int64_t>(args[2], decrement)) {
				return code::getErrorReply(code::status::invaild_argument);
			}

			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) {
				objectMap.update(std::move(args[1]), decrement);
				return code::getIntegerReply(decrement);
			}
			else {
				// ��visit��-increment
				auto ret = visit([decrement](auto& e)
				{
					return visitor::decrby(e, decrement);
				}, iter->second);

				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);
			}
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}

	}


	/*
	* 
	* @author: Lizezheng
	* mset:����һ�������ö����ֵ
	* date:2021/04/18
	* 
	*/
	std::optional<string> mset(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size()>1 && args.size()%2==0)
				return code::args_count_error;
			for (auto arg = args.begin() + 1; arg < args.end(); arg += 2)
			{
				objectMap.update(std::move(*arg), stringToObject(std::move(*(arg+1))));
			}
			return code::ok;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	*
	* @author: Lizezheng
	* mget:����һ���Ի�ȡ�����ֵ
	* date:2021/04/18
	*
	*/
	std::optional<string> mget(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
				return code::getMultiReply(args.begin() + 1, args.end(),
				[&ctx](vector<string>::iterator arg,std::back_insert_iterator<string> s)
				{
					auto&& objectMap = ctx.session.getObjectMap();
					auto iter = objectMap.find(*arg);
					if (iter != objectMap.end())
					{
						auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
						if (ret.first == code::status::success)
						{
							code::getBulkReplyTo(ret.second,s);
							return 1;
						}
					}
					code::getReplyTo(code::nil, s);
					return 1;
				});
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
	
	/*
	*
	* @author: Lizezheng
	* setrange:��һ��string�����丳ֵ
	* ���ַ���û����ô�����򲻴��ڣ�ʱ����ǰ�油0
	* date:2021/04/18
	*/
	std::optional<string> setrange(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			int64_t offset;
			if (!try_lexical_convert<int64_t>(args[2], offset) || offset < 0)
				return code::getErrorReply(code::status::invaild_argument);
			else
			{
				auto iter = objectMap.find(args[1]);
				size_t retlength;
				if (iter != objectMap.end())
				{
					//�������������Ϊ�ַ�����setrange�Ժ󣬶���һ������ַ�����
					auto ret = visit([&iter](auto& e)
					{
						return visitor::changeCodetoString(e, iter->second);
					}, iter->second);
					if (ret.first != code::status::success)
					{
						return code::getErrorReply(ret.first);
					}
					else
					{
						string& str = ret.second;
						str.reserve(offset + args[3].size());//Ԥ����ռ�
						if (str.length() >= (size_t)offset)//����ַ�����offset��
						{
							if (str.length() < offset + args[3].size())//����ַ���������
							{
								str.append(offset + args[3].size()-str.length(), '\0');//����
							}
							copy(args[3].begin(), args[3].end(), str.begin() + offset);
						}
						else
						{
							str.append(offset-str.length(), '\0');//����
							str.append(args[3]);//�����ַ���
						}
						retlength = str.length();
					}
				}
				else
				{
					auto ptr = make_unique<string>();
					string& str = *ptr;//��ȡָ����ָ���ַ���
					//����һ���ַ�������
					auto iter= objectMap.try_insert(std::move(args[1]), std::move(ptr));
					str.reserve(offset + args[3].size());//Ԥ����ռ�
					str.append(offset, '\0');//����
					str.append(args[3]);//�����ַ���
					retlength = str.length();
				}
				return code::getIntegerReply(retlength);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}





}