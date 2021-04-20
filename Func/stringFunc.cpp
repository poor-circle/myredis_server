#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/StringVisitor/get.h"
#include "../ObjectVisitor/StringVisitor/changeCodetoString.h"
#include "../ObjectVisitor/StringVisitor/incr.h"
#include "../ObjectVisitor/StringVisitor/decr.h"


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
			return nullopt;//返回空值
		}
	}

	//获取一个字符串
	optional<string> get(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())//找不到对应的key
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
			return nullopt;//返回空值
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
			if (iter == objectMap.end())//找不到对应的key
			{
				auto len = args[2].size();
				// 调用set函数
				objectMap.update(std::move(args[1]), stringToObject(std::move(args[2])));
				//set以后要返回长度
				return code::getIntegerReply(len);
			}
			else
			{
				//将对象编码强制转化为字符串
				auto ret = visit([iter](auto& e)
				{
					return visitor::changeCodetoString(e,iter->second);
				}, iter->second);
			
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}
				// append操作
				ret.second.append(args[2]);
				return code::getIntegerReply(ret.second.size());
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
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
				//找不到对应的key 返回0
				return code::getIntegerReply(0);
			}
			else
			{
				// 找到key取出
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
			return nullopt;//返回空值
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
				//找不到对应的key
				return code::nil;
			}
			else
			{
				// 找到key取出string
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.first);
				}
				// 使用stoi将start和end转换为Int
				// 不能转换会抛出invalid_argument异常
				// 超过返回会抛出out_of_range异常
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false || 
					try_lexical_convert(args[3], end) == false) {
					return code::getErrorReply(code::status::invaild_argument);
				}
				const int64_t strLen = ret.second.size();
				// 如果小于零 + strLen ，还小于0说明超过长度了，直接置0
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
			return nullopt;//返回空值
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
			return nullopt;//返回空值
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
			if (iter == objectMap.end())//找不到对应的key
			{
				// 返回nil
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				// 获取之前的值
				auto retstr = code::getBulkReply(ret.second);

				// 更新
				iter->second = stringToObject(std::move(args[2]));

				// 返回之前的值
				return retstr;
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
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
			return nullopt;//返回空值
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
				// 在visit层+1
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
			return nullopt;//返回空值
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
				// 在visit层+increment
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
			return nullopt;//返回空值
		}

	}

	/*
	* incrbyfloat
	* @author: tigerwang
	* date:2021/4/20
	* TODO: 对int64_t+float的处理 
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
			return nullopt;//返回空值
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
				// 在visit层+1
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
			return nullopt;//返回空值
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
				// 在visit层-increment
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
			return nullopt;//返回空值
		}

	}


	/*
	* 
	* @author: Lizezheng
	* mset:用于一次性设置多个键值
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
			return nullopt;//返回空值
		}
	}

	/*
	*
	* @author: Lizezheng
	* mget:用于一次性获取多个键值
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
			return nullopt;//返回空值
		}
	}
	
	/*
	*
	* @author: Lizezheng
	* setrange:对一个string的区间赋值
	* 当字符串没有那么长（或不存在）时，在前面补0
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
					//将对象编码设置为字符串（setrange以后，对象一定变成字符串）
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
						str.reserve(offset + args[3].size());//预申请空间
						if (str.length() >= (size_t)offset)//如果字符串比offset长
						{
							if (str.length() < offset + args[3].size())//如果字符串不够长
							{
								str.append(offset + args[3].size()-str.length(), '\0');//补零
							}
							copy(args[3].begin(), args[3].end(), str.begin() + offset);
						}
						else
						{
							str.append(offset-str.length(), '\0');//补零
							str.append(args[3]);//插入字符串
						}
						retlength = str.length();
					}
				}
				else
				{
					auto ptr = make_unique<string>();
					string& str = *ptr;//获取指针所指的字符串
					//插入一个字符串对象
					auto iter= objectMap.try_insert(std::move(args[1]), std::move(ptr));
					str.reserve(offset + args[3].size());//预申请空间
					str.append(offset, '\0');//补零
					str.append(args[3]);//插入字符串
					retlength = str.length();
				}
				return code::getIntegerReply(retlength);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}





}