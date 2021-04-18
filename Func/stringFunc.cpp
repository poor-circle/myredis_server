#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/StringVisitor/get.h"

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
			return nullopt;//返回空值
		}
	}

	//获取一个字符串
	optional<string> get(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//找不到对应的key
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
			return nullopt;//返回空值
		}
	}

	//append  
	/*
	* append 针对string对象
	* 如果 key 已经存在，并且值为字符串，那么这个命令会把 value 追加到原来值（value）的结尾，并返回string的长度。 
	* 如果 key 不存在，那么它将首先创建一个空字符串的key，再执行追加操作，这种情况 APPEND 将类似于 SET 操作。
	* code by tigerwang  2021/4/17 20:00
	*/
	std::optional<string> append(std::vector<string>&& args) noexcept 
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//找不到对应的key
			{
				auto len = args[2].size();
				// 调用set函数
				getObjectMap().update(std::move(args[1]), stringToObject(std::move(args[2])));
				//set以后要返回长度
				return code::getIntegerReply(len);
			}
			else
			{
				// 找到key取出 
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
			
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.second);
				}
				// append操作
				ret.second.append(args[2]);
				int64_t len = ret.second.size();
				iter->second = stringToObject(std::move(ret.second));
				return code::getIntegerReply(len);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}

	// strlen
	/* strlen 针对string类型的对象
	* 返回key的string类型value的长度。
	* 如果key对应的非string类型，就返回错误。
	* 如果key不存在返回0
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
				//找不到对应的key 返回0
				return code::getIntegerReply(0);
			}
			else
			{
				// 找到key取出
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
			return nullopt;//返回空值
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
			return nullopt;//返回空值
		}
	}
}