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
				// ���û�ҵ�,���½�һ�����б�,����ͷ������
				auto list = std::make_unique<deque<string>>();
				auto ret = visitor::lpush(list, pushContent);
				objectMap.update(std::move(args[1]), std::move(list));
				return code::getIntegerReply(ret.second);
			}
			else 
			{
				auto ret = visit([&pushContent](auto& e) 
				{
					return visitor::lpush(e,pushContent); 
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
			return nullopt;//���ؿ�ֵ
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
				// ���û�ҵ�,ʲô������
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&pushContent](auto& e)
				{
					return visitor::lpush(e, pushContent);
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
			return nullopt;//���ؿ�ֵ
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
				// ���û�ҵ�,�����ǿ�list ����0
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
			return nullopt;//���ؿ�ֵ
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
				// �Ҳ�����Ӧ��key����nil
				return code::nil;
			}
			else
			{
				// start,end ����Ӧ��lrange������ִ��
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
				for (auto iter = ret.second.begin(); iter != ret.second.end(); iter++) {
					auto str = fmt::format("{}\n", *iter);
					printf(str.c_str());
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
			return nullopt;//���ؿ�ֵ
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
				// ���û�ҵ�,���½�һ�����б�,����ͷ������
				auto list = std::make_unique<deque<string>>();
				auto ret = visitor::rpush(list, pushContent);
				objectMap.update(std::move(args[1]), std::move(list));
				return code::getIntegerReply(ret.second);
			}
			else
			{
				auto ret = visit([&pushContent](auto& e)
				{
					return visitor::rpush(e, pushContent);
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
			return nullopt;//���ؿ�ֵ
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
				// ���û�ҵ�,ʲô������
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&pushContent](auto& e)
				{
					return visitor::rpush(e, pushContent);
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
			return nullopt;//���ؿ�ֵ
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
			return nullopt;//���ؿ�ֵ
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
			return nullopt;//���ؿ�ֵ
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
				// source������
				return code::nil;
			}
			else
			{
				auto desIter = iter;
				if (args[1] != args[2]) {
					// ���source��destination��ͬ
					desIter = objectMap.find(args[2]);
					if (desIter == objectMap.end()) 
					{
						// destination�Ҳ��� �½�һ�����б�
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
			return nullopt;//���ؿ�ֵ
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
				// ��indexת��Ϊ����
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
			return nullopt;//���ؿ�ֵ
		}
	}

}