#include "stdafx.h"
#include "funcManager.h"
#include "allFunc.h"
namespace myredis
{

	const hash_map<string, func::funcPtr>& getfuncManager()
	{
		const static hash_map<string, func::funcPtr> manager
		{
			{"set",func::set },
			{"get",func::get }
			//regist here
		};
		return manager;
	}
}
