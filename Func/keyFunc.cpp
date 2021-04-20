#include "stdafx.h"
#include "func.h"
#include "keyFunc.h"
#include "../object.hpp"
#include "../code.h"

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
	std::optional<string> exists(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
				return code::getMultiReply(args.begin() + 1, args.end(),
					[](vector<string>::iterator& arg)
			{
				auto iter = getObjectMap().find(*arg);
				if (iter == getObjectMap().end())
				{
					return code::getIntegerReply(0);
				}
				else
				{
					return code::getIntegerReply(1);
				}
			});
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//返回空值
		}
	}
}