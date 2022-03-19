#pragma once
#include"stdc++.h"
#include"boost/container/list.hpp"
namespace myredis
{
	struct keyIter;
	class objectMap;
	struct keyInfo
	{
		using time_duration = std::chrono::milliseconds;
		friend struct keyIter;
		friend class objectMap;
	private:
		string str;
		time_duration liveTime;//ms from 1970/01/01 
	public:
		keyInfo(const string& str, time_duration liveTime = time_duration::max()) :str(str), liveTime(liveTime) {}
		keyInfo(string&& str, time_duration liveTime = time_duration::max()) :str(std::move(str)), liveTime(liveTime) {}
		bool isExpired()
		{
			return isSetExpiredTime() && liveTime <= getUnixMSDuration();
		}
		bool isSetExpiredTime()
		{
			return liveTime != time_duration::max();
		}
		time_duration getLiveTime() const { return liveTime; }
		time_duration getLiveTimeFromNow() const { return liveTime - getUnixMSDuration(); }
		const string& getStr() const { return str; }
		friend bool operator == (const keyInfo& key1, const keyInfo& key2)
		{
			return key1.str == key2.str;
		}
		//获取以s为单位的unix时间戳
		static std::chrono::seconds getUnixDuration(std::chrono::seconds sec=std::chrono::seconds(0))
		{
			return std::chrono::seconds(std::time(NULL))+sec;
		}
		//获取以ms为单位的unix时间戳
		static time_duration getUnixMSDuration(std::chrono::milliseconds ms = std::chrono::milliseconds(0))
		{
			auto gg = std::chrono::steady_clock::now().time_since_epoch() % std::chrono::seconds(1);
			auto g = std::chrono::milliseconds(std::chrono::seconds(std::time(NULL))) + ms + std::chrono::duration_cast<std::chrono::milliseconds>(gg);
			return g;
		}
	};

	struct keyIter
	{
		mutable boost::container::list<keyInfo>::iterator iter;
		friend bool operator == (const keyIter& iter1, const keyIter& iter2)
		{
			return *iter1.iter == *iter2.iter;
		}
		friend bool operator < (const keyIter& iter1, const keyIter& iter2)
		{
			if (iter1.iter->getLiveTime() == iter2.iter->getLiveTime())
				return iter1.iter->getStr() < iter2.iter->getStr();
			else
				return iter1.iter->getLiveTime() < iter2.iter->getLiveTime();
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
			return boost::container::hash_value(p.iter->getStr());
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
	

	//magic constexpr to get object from index;
	template <std::size_t I = 0>
	object object_from_index(std::size_t index) {
		if constexpr (I >= std::variant_size_v<object>)
			throw std::exception();//if outrange,throw exception
		else
			return index == 0
				? object{std::in_place_index<I>}
				: object_from_index<I + 1>(index - 1);
	}

	//将一个String转换为String类型的对象
	object stringToObject(string&& str);

	struct watchInfo;
	using watchMap = std::unordered_map<string, boost::container::list<watchInfo>>;
	using watcher = hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>;
	using watcherPtr = std::shared_ptr<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>>;

	struct watchInfo
	{
		int64_t sessionID;//线程ID
		watcherPtr nodes;
		//记录其他key上的监视器
		std::shared_ptr<std::function<std::optional<string>(const string&)>> op;
		//用户自定义的操作，返回值：bool：是否让队列中的下一个监视器继续执行，string:准备返回给客户端的string
		//参数：const string&:被监视的object的key名
		watchInfo(int64_t sessionID,
			watcherPtr nodes,
			std::shared_ptr<std::function<std::optional<string>(const string&)>> op) :
			sessionID(sessionID), nodes(nodes), op(op) {}
	};

	class objectMap //不要直接创建这个对象！(通过getObjectMap来获取）
	{
	private:
		boost::container::list<keyInfo> keylist;
		hash_map<keyIter, object> map,cowMAP;
		std::set<keyIter> expireHeap;//目前简化了实现，没有真正使用*堆*
		void _updateExpireTime(const keyIter& iter, keyInfo::time_duration ms);
	public:
		void updateExpireTime(const keyIter& iter,std::chrono::seconds sec);
		void updateExpireTime(const keyIter& iter, std::chrono::milliseconds ms);
		void updateExpireTime(const keyIter& iter, int64_t unix_ms_tp);
		void cancelExpireTime(const keyIter& iter);
		static asio::awaitable<void> expiredKeyCollecting();

		//直接修改数据库
		void update(keyInfo&& key,object&& obj);
		hash_map<keyIter, object>::iterator try_insert(keyInfo&& key, object&& obj);

		hash_map<keyIter, object>::iterator find(const string& str);
		hash_map<keyIter, object>::iterator find(string&& str);
		const object& find(boost::container::list<keyInfo>::const_iterator iter);


		size_t erase(string&& str);
		hash_map<keyIter, object>::iterator erase(hash_map<keyIter, object>::const_iterator iter);

		
		hash_map<keyIter, object>::const_iterator cbegin() const;
		hash_map<keyIter, object>::const_iterator cend() const;
		boost::container::list<keyInfo>::const_iterator keyBegin() const;
		boost::container::list<keyInfo>::const_iterator keyEnd()  const;
		void reserve(size_t sz);
		static objectMap& getObjectMap(int64_t index);
		static watchMap& getWatchMap(int64_t index);
		size_t size() const;
		template<typename Iter>
		static void addWatches(Iter begin, Iter end, int64_t dataBaseID, int64_t sessionID,watcherPtr& tab,
							   std::function<std::optional<string>(const string&)>&& op)
		{
			auto &map = objectMap::getWatchMap(dataBaseID);
			auto func = std::make_shared<std::function<std::optional<string>(const string&)>>(std::move(op));
			for (; begin != end; begin = std::next(begin))//遍历所有的key
			{
				auto iter = map.find(*begin);
				if (iter == map.end())//如果监视表中不存在这个key，则添加一个
				{
					iter = map.emplace(*begin, boost::container::list<watchInfo>()).first;
				}
				if (tab->find(&iter->second) != tab->end())//如果已经存在相同的key，直接跳过
					continue;
				iter->second.emplace_back(watchInfo(sessionID, tab, func));//向这个key的监视队列的队尾添加一个监视器
				tab->emplace(&iter->second, std::prev(iter->second.end()));//向
			}
		}
	};

	
}
