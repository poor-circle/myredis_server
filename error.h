#pragma once
#include "util/string.hpp"
namespace myredis::error
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
#define myredis_succeed(X) {myredis::error::code::success,X}
#define myredis_failed(X)  {myredis::error::code::##X,myredis::error::getErrorMessage(myredis::error::code::##X)}
}