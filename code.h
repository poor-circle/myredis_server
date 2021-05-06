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
		object_type_error,		//�������ʹ���
		value_overflow,			//��ֵ���
		invaild_argument,
		object_is_empty,		//����Ϊ�գ����в����������쳣
		index_out_of_range
		//add other error here
	};
	static string& getMessage(status i)			// �Ӵ������ȡ������Ϣ
	{
		static string error_message[] = // ���뱣֤message˳���code��ͬ!
		{
			"success",
			"object type error",
			"value overflow",
			"invalid argument or integer out of range",
			"object_is_empty",								// ���룬��ʹ��
			"index_out_of_range"
			//add other message here
		};
		return error_message[static_cast<std::size_t>(i)];
	}

	const string ok = "+OK\r\n";
	const string pong = "+PONG\r\n";
	const string args_count_error = "-Wrong args count\r\n";
	const string nil = "$-1\r\n";
	const string multi_nil = "*1\r\n$-1\r\n";
	const string no_password_error = "-Client sent AUTH, but no password is set\r\n";
	const string password_wrong_error = "-Password is wrong. Please try again\r\n";
	const string database_index_error = "-Invalid DB index\r\n";
	const string auth_error = "-Please login first.\r\n";
	const string regex_error = "-Illegal regex.\r\n";
	const string key_error = "-No such key.\r\n";
	const string command_error = "-No such command\r\n" ;
	const string subcommand_error = "-No such subcommand\r\n" ;
	const string args_illegal_error = "-Illegal args.\r\n";
	const string iterator_illegal_error = "-Illegal iterator.\r\n";
	const string server_exception_error = "-server internal exception.\r\n";

	template<typename T>
	static void getReplyTo(const T& str, std::back_insert_iterator<string> s)
	{
		fmt::format_to(s,FMT_COMPILE("{}"),str);
	}

	template<typename T>
	static string getBulkReply(const T& str)  //�����ظ�
	{
		string s;
		fmt::format_to
		(
			back_inserter(s),
			FMT_COMPILE("${}\r\n{}\r\n"),
			fmt::formatted_size(FMT_COMPILE("{}"), str),
			str
		);
		return s;
	}

	template<>
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

	template<>
	static string getBulkReply(const std::string_view& str)  //�����ظ�
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


	template<typename T>
	static void getBulkReplyTo(const T& str, std::back_insert_iterator<string> s)  //�����ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE("${}\r\n{}\r\n"),
			fmt::formatted_size(FMT_COMPILE("{}"), str),
			str
		);
	}

	template<>
	static void getBulkReplyTo(const string& str, std::back_insert_iterator<string> s)  //�����ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE("${}\r\n{}\r\n"),
			str.size(),
			str
		);
	}

	template<>
	static void getBulkReplyTo(const std::string_view& str, std::back_insert_iterator<string> s)  //�����ظ�
	{
		fmt::format_to
		(
			s,
			FMT_COMPILE("${}\r\n{}\r\n"),
			str.size(),
			str
		);
	}

	template<typename T>
	static string getSingleReply(const T& str) //״̬�ظ�
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

	template<typename T>
	static void getSingleReplyTo(const T& str, std::back_insert_iterator<string> s) //״̬�ظ�
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
	static string getMultiReplyByRange(Iter begin, Iter end,func lambda) //�������ظ�
	{
		string s="*00000000000000\r\n";
		auto index = s.size() - 3;
		int64_t cnt = 0;
		for (; begin != end; begin = std::next(begin))
		{
			auto ans = lambda(begin, back_inserter(s));
			if (ans < 0) break;
			else cnt += ans;
		}
			
		while (cnt)
		{
			s[index--] = '0'+cnt % 10;
			cnt /= 10;
		}
		return s;
	}

	template<typename T>
	static void _getMultiReplyWithOutHead(std::back_insert_iterator<string> s, const T& value) //�������ظ�
	{
		fmt::format_to(s, FMT_COMPILE("${}\r\n{}\r\n"), fmt::formatted_size(FMT_COMPILE("{}"), value) , value);
	}

	template<>
	static void _getMultiReplyWithOutHead(std::back_insert_iterator<string> s, const int64_t& value) //�������ظ�
	{
		getIntegerReplyTo(value,s);
	}

	

	template<typename T, class ...Args>
	static void _getMultiReplyWithOutHead(std::back_insert_iterator<string> s,const T& value, const Args&... args) //�������ظ�
	{
		fmt::format_to(s, FMT_COMPILE("${}\r\n{}\r\n"), fmt::formatted_size(FMT_COMPILE("{}"), value) , value);
		_getMultiReplyWithOutHead(s,args...);
	}

	template<class ...Args>
	static void _getMultiReplyWithOutHead(std::back_insert_iterator<string> s, const int64_t& value, const Args&... args) //�������ظ�
	{
		getIntegerReplyTo(value, s);
		_getMultiReplyWithOutHead(s, args...);
	}

	template<class ...Args>
	static void  getMultiReplyTo(std::back_insert_iterator<string> s,const Args&... args) //�������ظ�
	{
		fmt::format_to(s, FMT_COMPILE("*{}\r\n"), sizeof...(args));
		_getMultiReplyWithOutHead(s,args...);
		return;
	}

	template<class ...Args>
	static string getMultiReply(const Args&... args) //�������ظ�
	{
		string s;
		getMultiReplyTo(back_inserter(s),args...);
		return s;
	}


	template<class Iter, typename func,typename func2>
	static string getScanReply(Iter begin, Iter end, func lambda,func2 dislambda) //�������ظ�
	{
		string s = "*00000000000000\r\n";
		auto index = s.size() - 3;
		size_t cnt = 1;
		for (; begin != end; begin = std::next(begin))
		{
			auto ans = lambda(begin, back_inserter(s));
			if (ans < 0)
				break;
			else cnt += ans;
		}
		getBulkReplyTo(dislambda(begin), back_inserter(s));
		while (cnt)
		{
			s[index--] = '0' + cnt % 10;
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