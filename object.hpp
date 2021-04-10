#pragma once
#include"stdc++.h"
namespace myredis
{
	using object = std::variant  //myredis基本对象,基于std::variant实现的运行时多态，性能强于继承和虚函数
	<
		int64_t,											//整数编码,字符串对象
		double,												//浮点编码,字符串对象 
		std::unique_ptr<string>,							//字符串编码(sso编码或raw编码),字符串对象
		std::unique_ptr<hash_set<string>>,					//hash集合编码，set对象
															//TODO:intset编码，set对象
		std::unique_ptr<hash_map<string, string>>,			//hash映射编码，hash对象
															//TODO:ziplist编码，hash对象
		std::unique_ptr<key_ordered_map<double, string>>,	//有序映射集合编码，zset对象
															//TODO:ziplist编码，zset对象
		std::unique_ptr<deque<string>>						//双端队列编码，list对象
															//TODO:ziplist编码，list对象
	>;

	hash_map<string, object>& getObjectMap();

	//将一个String转换为String类型的对象
	object stringToObject(string&& str);

}
