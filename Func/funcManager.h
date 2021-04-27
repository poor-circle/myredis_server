#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis
{
	// 一个函数，用于查找函数名对应的函数
	const hash_map<string, func::funcInfo>& getfuncManager();

}

