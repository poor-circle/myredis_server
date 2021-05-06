#include "stdafx.h"
#include "func.h"
#include "keyFunc.h"
#include "../object.hpp"
#include "../code.h"
#include "../ObjectVisitor/keyVisitor/object_encode.h"
#include "../ObjectVisitor/keyVisitor/type.h"
#include "boost/container/list.hpp"

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
	/*
	* @author: tigerwang
	* date:2021/4/19
	* exists key [key ...]
	* ����key�Ƿ���ڡ�
	* ����ֵ
	* integer-reply�����µ��������
	* 1 ���key����
	* 0 ���key������
	*/
	std::optional<string> exists(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
			{
				for (auto iter=args.begin()+1;iter!=args.end();++iter)
				{
					auto ans = objectMap.find(*iter);
					if (ans == objectMap.end())
						return code::getIntegerReply(0);
				}
				return code::getIntegerReply(1);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* del key [key ...]
	* ����ɾ���ɹ���key����
	*/
	std::optional<string> del(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
			{
				int64_t cnt = 0;
				while (args.size()>1)
				{
					auto ans = objectMap.erase(std::move(args.back()));
					args.pop_back();
					if (ans) ++cnt;
				}
				return code::getIntegerReply(cnt);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* keys pattern
	* ����ƥ��������ʽpatter�����е�key
	*/
	std::optional<string> keys(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			else
			{
				std::regex rx;
				try
				{
					auto flag = regex_constants::ECMAScript ;
					if (objectMap.size() >= regexOpLowerBound)
						flag |= regex_constants::syntax_option_type::optimize;
					rx=regex(args[1].c_str(), flag);
				}
				catch (const exception &e)
				{
					return code::regex_error;
				}
				return code::getMultiReplyByRange(objectMap.begin(), objectMap.end(),
					[&rx](hash_map<keyIter, myredis::object>::iterator iter, auto s)
					{
						if (std::regex_match(iter->first.iter->getStr().c_str(), rx))
						{
							code::getBulkReplyTo(iter->first.iter->getStr(), s);
							return 1;
						}
						else 
							return 0;
					});
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* @author: lizezheng
	* date:2021/4/19
	* rename key newkey
	* ��key���������newkey�����򸲸�
	* ����ֵ��ok
	*/
	std::optional<string> rename(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			else
			{
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					myredis::object obj;
					auto expireTime = iter->first.iter->getLiveTime();
					swap(obj, iter->second);
					objectMap.erase(iter);
					objectMap.update(keyInfo(args[2],expireTime), std::move(obj));
					return code::ok;
				}
				else
					return code::key_error;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* rename key newkey
	* ��key���������newkey�����򷵻�0�����򷵻�1
	* ����ֵ������
	*/
	std::optional<string> renamenx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			else
			{
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					auto iter2 = objectMap.find(args[2]);
					int flag = iter2 == objectMap.end();
					if (flag)
					{
						myredis::object obj;
						auto expireTime = iter->first.iter->getLiveTime();
						swap(obj, iter->second);
						objectMap.erase(iter);
						objectMap.try_insert(keyInfo(args[2], expireTime), std::move(obj));
					}
					return code::getIntegerReply(flag);
				}
				else
					return code::key_error;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* object subcommand key
	* ִ��subcommand
	* ����ֵ��bulk string
	*/
	std::optional<string> object(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			else
			{
				boost::algorithm::to_lower(args[1]);
				if (args[1] == "encoding"sv)
				{
					auto iter = objectMap.find(args[2]);
					if (iter != objectMap.end())
					{
						return code::getBulkReply(
							visit([](auto& e) {return visitor::object_encode(e); }, iter->second));
					}
					else
						return code::key_error;
				}
				else if (args[1] == "refcount"sv)
				{
					return code::getIntegerReply(1);
				}
				//TODO:subcommand idletime
				else if (args[1] == "idletime"sv)
					return code::nil;
				else
					return code::subcommand_error;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/20
	* type key
	* ����key������
	* ����ֵ��bulk_string
	*/
	std::optional<string> type(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			else
			{
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					return code::getBulkReply(visit([](auto& e) {return visitor::type(e); },iter->second));
				}
				else
					return code::key_error;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	string scan_subcommand_parse(vector<string>& args, size_t index,size_t& cnt,string& pattern)
	{
		boost::algorithm::to_lower(args[index]);
		if (args[index] == "count"sv)
		{
			if (!try_lexical_convert(args[index + 1], cnt))
				return code::args_illegal_error;
		}
		else if (args[index]=="match"sv)
		{
			pattern = std::move(args[index + 1]);
		}
		else return code::subcommand_error;
		return "";
	}
	/*
	* @author: lizezheng
	* date:2021/4/20
	* scan 0|iter:key [match pattern] [count number]
	* ���ݼ�ֵ��ɨ��������ɸ�key
	* ����ֵ��multi_reply
	*/
	std::optional<string> scan(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size()!=2&&args.size()!=4&&args.size()!=6)
				return code::args_count_error;
			size_t count = 0,dcount=default_scan_count;
			string pattern,ret;
			size_t pos;
			regex rx;
			if (args[1] == "0"sv)
				pos = 0;
			else if (args[1].length()<5||string_view(args[1].c_str(), 5) != "iter:"sv)
			{
				return code::iterator_illegal_error;
			}
			else pos = -1;
			if (args.size() >= 4)
			{
				ret=scan_subcommand_parse(args, 2, count, pattern);
				if (!ret.empty()) return ret;
			}
			if (args.size() >= 6)
			{
				if (args[2] == args[4])
					return code::subcommand_error;
				scan_subcommand_parse(args, 4, count, pattern);
				if (!ret.empty()) return ret;
			}
			if (count == 0) count = dcount;
			else dcount = -1;//size_t_max
			if (!pattern.empty())
			{
				try
				{
					rx = regex(pattern.c_str());//TODO:compile optimize
				}
				catch (const exception& e)
				{
					return code::regex_error;
				}
			}
			auto ptr= objectMap.keyBegin();
			if (pos != 0)
			{
				auto iter = objectMap.find(args[1].substr(5));
				if (iter == objectMap.end())
					return code::iterator_illegal_error;
				else ptr = iter->first.iter;
			}
			return code::getScanReply(ptr, objectMap.keyEnd(),
				[&count, &dcount, &rx, &pattern](auto iter, auto inserter)
				{
					if (dcount == 0 || count == 0)
						return -1;
					--dcount;
					if (pattern.empty() || regex_match(iter->getStr().c_str(), rx))
					{
						fmt::format_to
						(
							inserter,
							FMT_COMPILE("${}\r\nkey:{}\r\n"),
							iter->getStr().size() + 4,
							iter->getStr()
						);
						--count;
						return 1;
					}
					return 0;
				},
				[&objectMap](auto iter)
				{
					if (iter == objectMap.keyEnd())
						return  string("0");
					else
						return  "iter:"+iter->getStr();
				});;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}


	/*
	* @author: lizezheng
	* date:2021/05/03
	* expire key timeout(seconds)
	* ����Ϊ��λ���ù���ʱ��
	* ����ֵ��integer
	*/
	std::optional<string> expire(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			else
			{
				int64_t sec;
				if (!try_lexical_convert(args[2], sec) || sec < 0)
				{
					return code::args_illegal_error;
				}
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					objectMap.updateExpireTime(iter->first, seconds(sec));
					return code::getIntegerReply(1);
				}
				else
					return code::getIntegerReply(0);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	std::optional<string> ttl(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			else
			{
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					if (iter->first.iter->isSetExpiredTime())
						return code::getIntegerReply(iter->first.iter->getLiveTimeFromNow()/1s);
					else
						return code::getIntegerReply(-1);
				}
				else
					return code::getIntegerReply(-2);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	std::optional<string> pttl(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			else
			{
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					if (iter->first.iter->isSetExpiredTime())
						return code::getIntegerReply(iter->first.iter->getLiveTimeFromNow() / 1ms);
					else
						return code::getIntegerReply(-1);
				}
				else
					return code::getIntegerReply(-2);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	std::optional<string> pexpire(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			else
			{
				int64_t ms;
				if (!try_lexical_convert(args[2], ms) || ms < 0)
				{
					return code::args_illegal_error;
				}
				auto iter = objectMap.find(args[1]);
				if (iter != objectMap.end())
				{
					objectMap.updateExpireTime(iter->first, milliseconds(ms));
					return code::getIntegerReply(1);
				}
				else
					return code::getIntegerReply(0);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
}