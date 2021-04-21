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
			add(strlen),
			add(getrange),
			add(setnx),
			add(getset),
			add(mset),
			add(mget),
			add(setrange),
			add(msetnx),
			add(exists),
			add(incr),
			add(quit),
			add(echo),
			add(auth),
			add(select),
			add(del),
			add(keys),
			add(rename),
			add(renamenx),
			add(object),
			add(incrby),
			add(incrbyfloat),
			add(decr),
			add(decrby),
			add(lpush),
			add(lpushx),
			add(llen),
			add(lrange)
			//regist func at here
			//{"name",func::name}
		};
		return manager;
	}

#undef add

}
