#include "stdafx.h"
#include "func.h"
#include "listFunc.h"
#include "../object.hpp"
#include "../code.h"
#include "../ObjectVisitor/ListVisitor/push.h"
#include "../ObjectVisitor/ListVisitor/llen.h"
#include "../ObjectVisitor/ListVisitor/lrange.h"
#include "../ObjectVisitor/ListVisitor/pop.h"
#include "../ObjectVisitor/ListVisitor/rpoplpush.h"
#include "../ObjectVisitor/ListVisitor/lset.h"
#include "../ObjectVisitor/ListVisitor/lindex.h"
#include "../ObjectVisitor/ListVisitor/ltrim.h"
#include "../ObjectVisitor/ListVisitor/lrem.h"
#include "../ObjectVisitor/ListVisitor/linsert.h"


namespace myredis::func {
#include"../namespace.i"

	/*
	* lpush
	* @author:tigerwang 
	* date:2021/4/21
	*/
	std::optional<string> lpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto& pushContent = args[2];
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) 
			{
				// 如果没找到,则新建一个空列表,并在头部插入
				auto list = std::make_unique<deque<string>>();
				auto ret = visitor::lpush(list, std::move(pushContent), args[1],ctx.session);
				objectMap.update(std::move(args[1]), std::move(list));
				return code::getIntegerReply(ret.second);
			}
			else 
			{
				auto ret = visit([&pushContent,&ctx,&args](auto& e)
				{
					return visitor::lpush(e,std::move(pushContent),args[1], ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}
	
	/*
	* lpushx
	* @author:tigerwang 
	* date:2021/4/21
	*/
	std::optional<string> lpushx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto& pushContent = args[2];
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// 如果没找到,什么都不做
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&pushContent,&args,&ctx](auto& e)
				{
					return visitor::lpush(e, std::move(pushContent),args[1],ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/* 
	* llen
	* @author:tigerwang
	* date:2021/4/21
	*/
	std::optional<string> llen(context&& ctx) noexcept
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
				// 如果没找到,看作是空list 返回0
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::llen(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* lrange
	* @author:tigerwang
	* date:2021/4/22
	*/
	std::optional<string> lrange(context&& ctx) noexcept
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
				// 找不到对应的key返回nil
				return code::nil;
			}
			else
			{
				// start,end 到对应的lrange函数里执行
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false ||
					try_lexical_convert(args[3], end) == false)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}

				auto ret = visit([start,end](auto& e)
				{
					return visitor::lrange(e, start, end);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				auto s= code::getMultiReply(ret.second.begin(), ret.second.end(),
					[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{
					code::getBulkReplyTo(*arg,s);
					return 1;
				});
				return s;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* rpush
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> rpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto& pushContent = args[2];
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// 如果没找到,则新建一个空列表,并在头部插入
				auto list = std::make_unique<deque<string>>();
				auto ret = visitor::rpush(list, std::move(pushContent),args[1],ctx.session);
				objectMap.update(std::move(args[1]), std::move(list));
				return code::getIntegerReply(ret.second);
			}
			else
			{
				auto ret = visit([&pushContent,&args,&ctx](auto& e)
				{
					return visitor::rpush(e, std::move(pushContent),args[1],ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* rpushx
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> rpushx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto& pushContent = args[2];
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// 如果没找到,什么都不做
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&pushContent,&args,&ctx](auto& e)
				{
					return visitor::rpush(e, std::move(pushContent),args[1],ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* lpop
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> lpop(context&& ctx) noexcept
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
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::lpop(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::nil;
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* rpop
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> rpop(context&& ctx) noexcept
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
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::rpop(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::nil;
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* rpoplpush source destination
	* @author:tigerwang 
	* date:2021/4/23
	*/
	std::optional<string> rpoplpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// source不存在
				return code::nil;
			}
			else
			{
				auto desIter = iter;
				if (args[1] != args[2]) {
					// 如果source和destination不同
					desIter = objectMap.find(args[2]);
					if (desIter == objectMap.end()) 
					{
						// destination找不到 新建一个空列表
						object list = std::make_unique<deque<string>>();
						auto ret = visit([&list](auto& e)
						{
							return visitor::rpoplpush(e, list);
						}, iter->second);
						objectMap.update(std::move(args[2]), std::move(list));
						
						if (ret.first != code::status::success)
						{
							return code::nil;
						}
						return code::getBulkReply(ret.second);
					}
				}
				auto ret = visit([&desIter](auto& e)
				{
					return visitor::rpoplpush(e, desIter->second);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::nil;
				}
				return code::getBulkReply(ret.second);
				
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}
	
	/*
	* lset key index value
	* @author:tigerwang
	* date:2021/4/25
	*/
	std::optional<string> lset(context&& ctx) noexcept
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
				return code::key_error;
			}
			else
			{
				// 将index转换为整数
				int64_t index;
				if (try_lexical_convert(args[2], index) == false
					)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				auto& value = args[3];
				auto ret = visit([index,&value](auto& e)
				{
					return visitor::lset(e,index, value);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::ok;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* lindex key index
	* @author:tigerwang
	* date:2021/4/25
	*/
	std::optional<string> lindex(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::key_error;
			}
			else
			{
				// 将index转换为整数
				int64_t index;
				if (try_lexical_convert(args[2], index) == false
					)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				auto ret = visit([index](auto& e)
				{
					return visitor::lindex(e, index);
				}, iter->second);

				if (ret.first == code::status::index_out_of_range) {
					return code::nil;
				}
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* ltrim key start stop
	* @author:tigerwang
	* date:2021/4/25
	*/
	std::optional<string> ltrim(context&& ctx) noexcept
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
				// 找不到对应的key返回nil
				return code::key_error;
			}
			else
			{
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false ||
					try_lexical_convert(args[3], end) == false)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}

				auto ret = visit([start, end](auto& e)
				{
					return visitor::ltrim(e, start, end);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::ok;

			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* lrem key count value
	* @author:tigerwang
	* date:2021/4/26
	*/
	std::optional<string> lrem(context&& ctx) noexcept
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
				// 找不到对应的key返回nil
				return code::getIntegerReply(0);
			}
			else
			{
				// start,end 到对应的lrange函数里执行
				int64_t count;
				if (try_lexical_convert(args[2], count) == false )
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				auto& value = args[3];
				auto ret = visit([count, &value](auto& e)
				{
					return visitor::lrem(e, count, value);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);

			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* linsert key BEFORE|AFTER pivot value
	* @author:tigerwang
	* date:2021/4/26
	*/
	std::optional<string> linsert(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 5)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::key_error;
			}
			else
			{
				auto& insertType = args[2];
				boost::algorithm::to_lower(insertType);
				if (insertType != "before" && insertType!="after")
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				
				auto& pivot = args[3];
				auto& value = args[4];
				auto ret = visit([&insertType,&pivot, &value](auto& e)
				{ 
					return visitor::linsert(e, insertType, pivot, value);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);

			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}

	/*
	* block and wait if list isnt empty
	* @author:lizezheng
	* date:2021/4/27
	*/
	std::optional<string> blpop(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() <3)
				return code::args_count_error;
			int64_t sec;
			if (!try_lexical_convert(args.back(), sec) || sec < 0)//得到阻塞时间
			{
				return code::args_illegal_error;
			}
			//如果现在已经有可以返回的string，直接返回
			for (auto arg = args.begin() + 1, endarg = args.end() - 1; arg != endarg; ++arg)
			{
				auto iter = objectMap.find(*arg);
				if (iter != objectMap.end())
				{
					auto ret = visit([](auto& e)
					{
						return visitor::lpop(e);
					}, iter->second);
					if (ret.first == code::status::success)
					{
						//返回key的名字和key的值
						return "*2\r\n" + code::getBulkReply(*arg) + code::getBulkReply(ret.second);
					}
					else if (ret.first == code::status::object_type_error)
					{
						return code::getErrorReply(code::status::object_type_error);
					}
				}
			}
			//否则，准备进入阻塞态
			//添加监视器，用于在合适的情况下唤醒会话
			auto watch_list=objectMap::addWatches(args.begin() + 1, args.end() - 1,ctx.session.getDataBaseID(),ctx.session.getSessionID(),
			[&objectMap](const string& keyName)
			{
				auto iter = objectMap.find(keyName);
				optional<string> ret=nullopt;//返回空，代表失败，监视器将继续监视
				//返回非空值，代表监视成功，该线程的所有监视器将被删除，并将字符串返回给客户端，退出阻塞状态
				//如果队列为空，返回空，继续监视。如果队列不为空，弹出队列的队首，停止阻塞，将其返回给客户端
				if (iter != objectMap.end())
				{
					auto ans = visit([](auto& e)
					{
						return visitor::lpop(e);
					}, iter->second);
					if (ans.first == code::status::success)
					{
						//返回key的名字和key的值
						ret= "*2\r\n"+code::getBulkReply(keyName)+code::getBulkReply(ans.second);
					}
				}
				return ret;
			});
			//将会话设为阻塞态,并设置阻塞时间
			ctx.session.setBlocked(code::multi_nil,seconds(sec), watch_list);
			return nullopt;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//返回空值
		}
	}
}