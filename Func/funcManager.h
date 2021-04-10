#pragma once
#include "../stdafx.h"
#include "func.h"
namespace myredis
{
	const hash_map<string, func::funcPtr>& getfuncManager();

}

