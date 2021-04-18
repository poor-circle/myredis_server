#include "stdafx.h"
#include "funcManager.h"
#include "allFunc.h"
namespace myredis
{
	
#define add(X) {#X,func::##X}

	const hash_map<string, func::funcPtr>& getfuncManager()
	{
		const static hash_map<string, func::funcPtr> manager
		{
			//在这里注册每一种func 
			add(get),
			add(set),
			add(ping),
			add(append),
			add(strlen)
			//regist func at here
			//{"name",func::name}
		};
		return manager;
	}

#undef add

}
