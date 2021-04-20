#pragma once
#include "util/string.hpp"
#include "fmt/core.h"
#include "fmt/compile.h"
#include <string_view>
namespace myredis::code
{
	enum class status			//����������
	{
		success,
		object_type_error,	//�������ʹ���
		value_overflow,		//��ֵ���
		invaild_argument,
		//add other error here
	};
	static string& getMessage(status i)			// �Ӵ������ȡ������Ϣ
	{
		static string error_message[] = // ���뱣֤message˳���code��ͬ!
		{
			"success",
			"object type error",
			"value overflow",
			"invalid argument or integer out of range"
			//add other message here
		};
		return error_message[static_cast<std::size_t>(i)];
	}

	const string ok = "+OK\r\n";
	const string pong = "+PONG\r\n";
	const string args_count_error = "-Wrong args count\r\n";
	const string nil = "$-1\r\n";
	const string no_password_error = "-Client sent AUTH, but no password is set\r\n";
	const string password_wrong_error = "-Password is wrong. Please try again\r\n";
	const string database_index_error = "-Invalid DB index\r\n";
	const string auth_error = "-Please login first.\r\n";
	const string regex_error = "-Illegel regex.\r\n";
	const string key_error = "-No such key.\r\n";
	const string command_error = { "-no such command\r\n" };
	const string subcommand_error = { "-no such subcommand\r\n" };

	static void getReplyTo(const std::string_view str, std::back_insert_iterator<string> s)
	{
		copy(str.begin(), str.end(), s);
	}

	static string getBulkReply(const std::string_view str)  //�����ظ�
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

	static void getBulkReplyTo(std::string_view str,std::back_insert_iterator<string> s)  //�����ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE("${}\r\n{}\r\n"),
			str.size(),
			str
		);
	}

	static string getSingleReply(const std::string_view str) //״̬�ظ�
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

	static void getSingleReplyTo(const std::string_view str , std::back_insert_iterator<string> s) //״̬�ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE("+{}\r\n"),
			str
		);
	}

	static string getErrorReply(status errorInfo) //����ظ�
	{
		string s;
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE("-{}\r\n"),
			getMessage(errorInfo)
		);
		return s;
	}

	static void getErrorReplyTo( status errorInfo, std::back_insert_iterator<string> s) //����ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE("-{}\r\n"),
			getMessage(errorInfo)
		);
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

	static void getIntegerReplyTo(int64_t number,std::back_insert_iterator<string> s) //�����ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE(":{}\r\n"),
			number
		);
	}

	/*
	*
	* @author: Lizezheng
	* getMultiReply:���ڸ�ʽ���������ظ�
	* date:2021/04/18
	* begin,end:һ�Ե�����
	* lambda:һ������������һ�������������һ��Iter������������һ������ֵ���������Ļظ�����
	*/
	template<class Iter,typename func>
	static string getMultiReply(Iter begin, Iter end,func lambda) //�������ظ�
	{
		string s="*00000000000000\r\n";
		auto index = s.size() - 3;
		int64_t cnt = 0;
		for (; begin != end; begin = std::next(begin))
			if (lambda(begin, back_inserter(s))) ++cnt;
		while (cnt)
		{
			s[index--] = '0'+cnt % 10;
			cnt /= 10;
		}
		return s;
	}

#define printlog(e)\
		fmt::print\
		(\
			stderr,\
			"myredis error at function \"{}\"\nexception info:{}\n",\
			__func__,\
			e.what()\
		);
	//add error info here;


#define myredis_succeed(X) {myredis::code::status::success,X}
#define myredis_failed(X)  {myredis::code::status::##X,myredis::code::getMessage(myredis::code::status::##X)}
}