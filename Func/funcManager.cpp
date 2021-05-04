#include "stdafx.h"
#include "funcManager.h"
#include "allFunc.h"
namespace myredis
{
	
#define read(X) {#X,{func::##X,func::funcType::read}}

#define write(X) {#X,{func::##X,func::funcType::write}}

#define connect(X) {#X,{func::##X,func::funcType::connect}}

#define blocked(X) {#X,{func::##X,func::funcType::blocked}}

#define pubsub(X) {#X,{func::##X,func::funcType::pubsub}}

	const hash_map<string, func::funcInfo>& getfuncManager()
	{
		const static hash_map<string, func::funcInfo> manager
		{
			//在这里注册每一种func 

			// 读
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
			// 写
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

			write(setex),
			write(expire),

			//连接状态
			connect(ping),
			connect(quit),
			connect(echo),
			connect(auth),
			connect(select),

			//监听-阻塞函数
			blocked(blpop),
			blocked(brpop),
			blocked(brpoplpush),
			//支持异步执行的订阅-发布（pubsub）函数

			pubsub(wait),
			pubsub(test),
			pubsub(subscribe)
			//regist func at here
			//{"name",func::name}
		};
		return manager;
	}

#undef add

}
