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

			// ��
			add(get),
			add(strlen),
			add(getrange),
			add(mget),
			add(exists),
			add(keys),
			add(llen),
			add(lrange),
			add(lindex),
			add(ltrim),

			// д
			add(set),
			add(append),
			add(setnx),
			add(getset),
			add(mset),
			add(setrange),
			add(msetnx),
			add(incr),
			add(del),
			add(rename),
			add(renamenx),
			add(incrby),
			add(incrbyfloat),
			add(decr),
			add(decrby),
			add(lpush),
			add(lpushx),
			add(rpush),
			add(rpushx),
			add(lpop),
			add(rpop),
			add(rpoplpush),
			add(lset),
			add(lrem),
			add(linsert),

			// ����
			add(ping),
			add(quit),
			add(echo),
			add(auth),
			add(select),
			add(object),
			add(type),
			add(scan)
			
			//regist func at here
			//{"name",func::name}
		};
		return manager;
	}

#undef add

}
