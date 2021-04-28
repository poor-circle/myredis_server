#include "stdafx.h"
#include "funcManager.h"
#include "allFunc.h"
namespace myredis
{
	
#define read(X) {#X,{func::##X,func::funcType::read}}
#define write(X) {#X,{func::##X,func::funcType::write}}
#define connect(X) {#X,{func::##X,func::funcType::connect}}
#define blocked(X) {#X,{func::##X,func::funcType::blocked}}

	const hash_map<string, func::funcInfo>& getfuncManager()
	{
		const static hash_map<string, func::funcInfo> manager
		{
			//������ע��ÿһ��func 

			// ��
			read(get),
			read(strlen),
			read(getrange),
			read(mget),
			read(exists),
			read(keys),
			read(llen),
			read(lrange),
			read(lindex),
			read(ltrim),
			read(scard),
			read(smembers),
			read(scan),
			read(object),
			read(type),
			read(sismember),
			// д
			write(set),
			write(append),
			write(setnx),
			write(getset),
			write(mset),
			write(setrange),
			write(msetnx),
			write(incr),
			write(del),
			write(rename),
			write(renamenx),
			write(incrby),
			write(incrbyfloat),
			write(decr),
			write(decrby),
			write(lpush),
			write(lpushx),
			write(rpush),
			write(rpushx),
			write(lpop),
			write(rpop),
			write(rpoplpush),
			write(lset),
			write(lrem),
			write(linsert),
			write(sadd),
			write(srem),

			//����״̬
			connect(ping),
			connect(quit),
			connect(echo),
			connect(auth),
			connect(select),
			
			//����-��������
			blocked(blpop)

			
			//regist func at here
			//{"name",func::name}
		};
		return manager;
	}

#undef add

}
