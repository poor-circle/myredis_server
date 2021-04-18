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
			//������ע��ÿһ��func 
			add(get),
			add(set),
			add(ping),
			add(append),
			add(strlen),
			add(getrange),
			add(setnx),
			add(getset)
			//regist func at here
			//{"name",func::name}
		};
		return manager;
	}

#undef add

}
