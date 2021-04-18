#pragma once
#include "util/string.hpp"
#include "fmt/core.h"
#include "fmt/compile.h"
#include <string_view>
namespace myredis::code
{
	enum class status			//函数错误码
	{
		success,
		object_type_error	//对象类型错误
		//add other error here
	};
	static string& getErrorMessage(status i)			// 从错误码获取错误信息
	{
		static string error_message[] = // 必须保证message顺序和code相同!
		{
			"success",
			"object type error"
			//add other message here
		};
		return error_message[static_cast<std::size_t>(i)];
	}

	const string succees_reply = "+OK\r\n";
	const string pong = "+PONG\r\n";
	const string args_count_error = "-error:wrong args count\r\n";
	const string nil = "$-1\r\n";

	static string getBulkReply(const std::string_view str)  //批量回复
	{
		string s;
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE("${}\r\n{}\r\n"),
			str.size(),
			str
		);
		return s;
	}

	static string getSingleReply(const std::string_view str) //状态回复
	{
		string s;
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE("+{}\r\n"),
			str
		);
		return s;
	}

	static string getErrorReply(const std::string_view errorInfo) //错误回复
	{
		string s;
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE("-{}\r\n"),
			errorInfo
		);
		return s;
	}

	static string getIntegerReply(int64_t number) //整数回复
	{
		string s;
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE(":{}\r\n"),
			number
		);
		return s;
	}
	/*
	*
	* @author: Lizezheng
	* getMultiReply:用于格式化多批量回复
	* date:2021/04/18
	* begin,end:一对迭代器
	* lambda:一个函数，接收一个迭代器，返回一个string&
	*/
	template<class Iter,typename func>
	static string getMultiReply(Iter begin, Iter end,func lambda) //多批量回复
	{
		string s;
		auto dis = std::distance(begin, end);
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE("*{}\r\n"),
			dis
		);
		for (; begin != end; begin = std::next(begin))
		{
			fmt::format_to
			(
				back_inserter(s),
				FMT_COMPILE("{}"),
				lambda(begin)
			);
		}
		return s;
	}

	//add error info here;


#define myredis_succeed(X) {myredis::code::status::success,X}
#define myredis_failed(X)  {myredis::code::status::##X,myredis::code::getErrorMessage(myredis::code::status::##X)}
}