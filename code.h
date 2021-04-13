#pragma once
#include "util/string.hpp"
#include "fmt/core.h"
#include "fmt/compile.h"
namespace myredis::code
{
	enum class code			//����������
	{
		success,
		object_type_error	//�������ʹ���
		//add other error here
	};
	static string& getErrorMessage(code i)			// �Ӵ������ȡ������Ϣ
	{
		static string object_type_error_message[] = // ���뱣֤message˳���code��ͬ!
		{
			"success",
			"object type error"
			//add other message here
		};
		return object_type_error_message[static_cast<std::size_t>(i)];
	}

	const string succeed = "+OK\r\n";
	const string args_count_error = "-error:wrong args count\r\n";
	const string key_search_error = "-error:no such key\r\n";

	static string getBulkReply(const string& str)  //�����ظ�
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

	static string getSingleReply(const string& str) //״̬�ظ�
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

	static string getErrorReply(int64_t errorInfo) //����ظ�
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

	static string getIntegerReply(int64_t number) //�����ظ�
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

	template<class Iter>
	static string getMultiReply(Iter begin, Iter end) //�������ظ�
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
				FMT_COMPILE("${}\r\n{}\r\n"),
				begin.size(),
				begin
			);
		}
		return s;
	}

	//add error info here;


#define myredis_succeed(X) {myredis::code::code::success,X}
#define myredis_failed(X)  {myredis::code::code::##X,myredis::code::getErrorMessage(myredis::code::code::##X)}
}