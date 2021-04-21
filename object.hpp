#pragma once
#include"stdc++.h"
#include"boost/container/list.hpp"
namespace myredis
{
	struct keyIter
	{
		mutable boost::container::list<string>::iterator iter;
		friend bool operator == (const keyIter& iter1, const keyIter& iter2)
		{
			return *iter1.iter == *iter2.iter;
		}
		template<typename T>
		keyIter(T && v):iter(std::forward<T>(v)){}
	};
}
namespace std
{
	template<>
	struct hash<myredis::keyIter>
	{
		std::size_t operator()(myredis::keyIter const& p) const
		{
			return boost::container::hash_value(*p.iter);
		}
	};
	//添加标准库对于myredis::string的哈希支持
}

namespace myredis
{

	//myredis中：key一定是原始字符串，value则可能是各种对象：

	//value的详细类型：
	//1.一共有5种对象
	//2.每种对象可以有不同的编码
	//目前，string实现了四种编码，其他对象目前只实现一种编码
	
	//unique_ptr：一种智能指针，性能和原始指针相等，能自动释放指向的对象的内存
	//unique_ptr不支持复制（只能有一个指针指向对象）

	using object = std::variant  //myredis基本对象,基于std::variant实现的运行时多态，性能强于继承和虚函数
	<
		int64_t,											//整数编码,字符串对象
		double,												//浮点编码,字符串对象 
		std::unique_ptr<string>,							//字符串编码(可以是embstr编码或raw编码),字符串对象
		std::unique_ptr<hash_set<string>>,					//hash集合编码，set对象
															//TODO:intset编码，set对象
		std::unique_ptr<hash_map<string, string>>,			//hash映射编码，hash对象
															//TODO:ziplist编码，hash对象
		std::unique_ptr<key_ordered_map<double, string>>,	//有序映射集合编码，zset对象
															//TODO:ziplist编码，zset对象
		std::unique_ptr<deque<string>>						//双端队列编码，list对象
															//TODO:ziplist编码，list对象
	>;
	

	//将一个String转换为String类型的对象
	object stringToObject(string&& str);

	class objectMap
	{
	private:
		boost::container::list<string> keylist;
		hash_map<keyIter, object> map;
		//不要直接创建这个对象！(通过getObjectMap来获取）
	public:
		//插入(更新)一个key
		void update(string&& str,object&& obj);
		hash_map<keyIter, object>::iterator try_insert(string&& str, object&& obj);
		hash_map<keyIter, object>::iterator find(string& str);
		hash_map<keyIter, object>::iterator find(string&& str);
		hash_map<keyIter, object>::iterator begin();
		hash_map<keyIter, object>::iterator end();
		boost::container::list<string>::iterator keyBegin();
		boost::container::list<string>::iterator keyEnd();
		size_t erase(string&& str);
		hash_map<keyIter, object>::iterator erase(hash_map<keyIter, object>::const_iterator iter);
		static objectMap& getObjectMap(size_t index);
		size_t size() const;
	};
}
