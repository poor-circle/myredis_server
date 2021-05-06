#include "stdafx.h"
#include "func.h"
#include "keyFunc.h"
#include "../object.hpp"
#include "../code.h"
#include "../ObjectVisitor/keyVisitor/object_encode.h"
#include "../ObjectVisitor/keyVisitor/type.h"
#include "boost/container/list.hpp"

//func层相当于spring boot的controller层
//通过controller调用visitor层
//调用格式：visit([](auto& e) {return visitor::funcName(e); }, object)
//visitor::funcName：你要调用的函数名
//object：你要操作的对象类型
namespace myredis::func
{
#include"../namespace.i"

	//设置一个字符串

	//无阻塞：每一个函数应该尽可能保证不出现阻塞

	//noexcept:每一个函数都保证自身不抛出任何异常。即使有一个操作崩溃，也能保证不影响其他操作
	/*
	* @author: tigerwang
	* date:2021/4/19
	* exists key [key ...]
	* 返回key是否存在。
	* 返回值
	* integer-reply，如下的整数结果
	* 1 如果key存在
	* 0 如果key不存在
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
			return nullopt;//返回空值
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* del key [key ...]
	* 返回删除成功的key个数
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
			return nullopt;//返回空值
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* keys pattern
	* 返回匹配正则表达式patter的所有的key
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
			return nullopt;//返回空值
		}
	}

	/*
	* @author: lizezheng
	* date:2021/4/19
	* rename key newkey
	* 给key改名，如果newkey存在则覆盖
	* 返回值：ok
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
			return nullopt;//返回空值
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* rename key newkey
	* 给key改名，如果newkey存在则返回0，否则返回1
	* 返回值：整数
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
			return nullopt;//返回空值
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/19
	* object subcommand key
	* 执行subcommand
	* 返回值：bulk string
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
			return nullopt;//返回空值
		}
	}
	/*
	* @author: lizezheng
	* date:2021/4/20
	* type key
	* 返回key的类型
	* 返回值：bulk_string
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
			return nullopt;//返回空值
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
	* 根据键值，扫描后面若干个key
	* 返回值：multi_reply
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
			return nullopt;//返回空值
		}
	}


	/*
	* @author: lizezheng
	* date:2021/05/03
	* expire key timeout(seconds)
	* 以秒为单位设置过期时间
	* 返回值：integer
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
			return nullopt;//返回空值
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
			return nullopt;//返回空值
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
			return nullopt;//返回空值
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
			return nullopt;//返回空值
		}
	}
}