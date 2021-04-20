#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/StringVisitor/get.h"
#include "../ObjectVisitor/StringVisitor/changeCodetoString.h"
#include "../ObjectVisitor/StringVisitor/incr.h"


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
	* @author:tigerwang
	* date:2021/4/18
	* append 针对string对象
	* 如果 key 已经存在，并且值为字符串，那么这个命令会把 value 追加到原来值（value）的结尾，并返回string的长度。 
	* 如果 key 不存在，那么它将首先创建一个空字符串的key，再执行追加操作，这种情况 APPEND 将类似于 SET 操作。
	* created by tigerwang  2021/4/17 20:00
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
	* @author:tigerwang
	* date:2021/4/18
	* strlen 针对string类型的对象
	* 返回key的string类型value的长度。
	* 如果key对应的非string类型，就返回错误。
	* 如果key不存在返回0
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
	* @author:tigerwang
	* date:2021/4/18
	* usage: GETRANGE key start end
	* 警告：这个命令是被改成GETRANGE的，在小于2.0的Redis版本中叫SUBSTR。
	* 返回key对应的字符串value的子串，这个子串是由start和end位移决定的（两者都在string内）
	* 可以用负的位移来表示从string尾部开始数的下标。所以-1就是最后一个字符，-2就是倒数第二个，以此类推。
	* 这个函数处理超出范围的请求时，都把结果限制在string内。
	* 返回值 bulk-reply
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
	* @author:tigerwang
	* date:2021/4/18
	* 将key设置值为value，
	* 如果key不存在，这种情况下等同SET命令。 
	* 当key存在时，什么也不做。
	* SETNX是”SET if Not eXists”的简写。
	* 
	* 返回值： Integer Reply
	* 1 如果key被设置了
	* 0 如果key没有被设置
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
	* @author:tigerwang
	* date:2021/4/18
	* 自动将key对应到value并且返回原来key对应的value。
	* 如果key存在但是对应的value不是字符串，就返回错误。
	* 返回值： bulk-string-reply
	* 返回之前的旧值，如果之前Key不存在将返回nil。
	* created by tigerwang	date:2021/4/18
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
	* @author: tigerwang 
	* date:2021/4/19
	* msetnx 
	* 对应给定的keys到他们相应的values上。只要有一个key已经存在，MSETNX一个操作都不会执行。
	* 由于这种特性，MSETNX可以实现要么所有的操作都成功，要么一个都不执行，
	* 这样可以用来设置不同的key，来表示一个唯一的对象的不同字段。
	* MSETNX是原子的，所以所有给定的keys是一次性set的。
	* 客户端不可能看到这种一部分keys被更新而另外的没有改变的情况。
	* 
	* 返回值
	* integer-reply，只有以下两种值：
	* 1 如果所有的key被set
	* 0 如果没有key被set(至少其中有一个key是存在的)
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
	* @author: tigerewang
	* date:2021/4/19
	* incr
	* 对存储在指定key的数值执行原子的加1操作。
	* 如果指定的key不存在，那么在执行incr操作之前，会先将它的值设定为0。
	* 如果指定的key中存储的值不是字符串类型或者存储的字符串类型不能表示为一个整数，
	* 那么执行这个命令时服务器会返回一个错误( ERR value is not an integer or out of range)。
	* 这个操作仅限于64位的有符号整型数据。
	* 注意: 由于redis并没有一个明确的类型来表示整型数据，所以这个操作是一个字符串操作。
	* 执行这个操作的时候，key对应存储的字符串被解析为10进制的64位有符号整型数据。
	* 
	* 返回值
	* integer-reply:执行递增操作后key对应的值。
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