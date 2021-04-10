#pragma once
#include"../stdafx.h"
namespace myredis::func
{
	//redis命令类型：
	//返回值：string，用于返回给客户端
	//参数：一个引用类型，指向一个vector<string>
	//保证所有异常都在内部捕获，不抛出任何异常
	
	using funcPtr = std::optional<boost::container::string>(*)(std::vector<boost::container::string>&&) noexcept;
}