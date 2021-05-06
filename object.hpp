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
			return isSetExpiredTime() && liveTime <= std::chrono::steady_clock::now().time_since_epoch();
		}
		bool isSetExpiredTime()
		{
			return liveTime != time_duration::max();
		}
		time_duration getLiveTime() const { return liveTime; }
		time_duration getLiveTimeFromNow() const { return liveTime - std::chrono::duration_cast<time_duration>(std::chrono::steady_clock::now().time_since_epoch()); }
		const string& getStr() const { return str; }
		friend bool operator == (const keyInfo& key1, const keyInfo& key2)
		{
			return key1.str == key2.str;
		}
		//��ȡ��msΪ��λ��unixʱ���
		static time_duration getUnixMSDuration(std::chrono::seconds sec)
		{
			auto st = std::chrono::duration_cast<time_duration>(std::chrono::steady_clock::now().time_since_epoch()) + sec;
			return st;
		}
		//��ȡ��msΪ��λ��unixʱ���
		static time_duration getUnixMSDuration(std::chrono::milliseconds sec)
		{
			return std::chrono::duration_cast<time_duration>(std::chrono::steady_clock::now().time_since_epoch() + sec);
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
	//��ӱ�׼�����myredis::string�Ĺ�ϣ֧��
}

namespace myredis
{

	//myredis�У�keyһ����ԭʼ�ַ�����value������Ǹ��ֶ���

	//value����ϸ���ͣ�
	//1.һ����5�ֶ���
	//2.ÿ�ֶ�������в�ͬ�ı���
	//Ŀǰ��stringʵ�������ֱ��룬��������Ŀǰֻʵ��һ�ֱ���
	
	//unique_ptr��һ������ָ�룬���ܺ�ԭʼָ����ȣ����Զ��ͷ�ָ��Ķ�����ڴ�
	//unique_ptr��֧�ָ��ƣ�ֻ����һ��ָ��ָ�����

	using object = std::variant  //myredis��������,����std::variantʵ�ֵ�����ʱ��̬������ǿ�ڼ̳к��麯��
	<
		int64_t,											//��������,�ַ�������
		double,												//�������,�ַ������� 
		std::unique_ptr<string>,							//�ַ�������(������embstr�����raw����),�ַ�������
		std::unique_ptr<hash_set<string>>,					//hash���ϱ��룬set����
															//TODO:intset���룬set����
		std::unique_ptr<hash_map<string, string>>,			//hashӳ����룬hash����
															//TODO:ziplist���룬hash����
		std::unique_ptr<key_ordered_map<double, string>>,	//����ӳ�伯�ϱ��룬zset����
															//TODO:ziplist���룬zset����
		std::unique_ptr<deque<string>>						//˫�˶��б��룬list����
															//TODO:ziplist���룬list����
	>;
	

	//��һ��Stringת��ΪString���͵Ķ���
	object stringToObject(string&& str);

	struct watchInfo;
	using watchMap = std::unordered_map<string, boost::container::list<watchInfo>>;
	using watcher = hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>;
	using watcherPtr = std::shared_ptr<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>>;

	struct watchInfo
	{
		size_t sessionID;//�߳�ID
		watcherPtr nodes;
		//��¼����key�ϵļ�����
		std::shared_ptr<std::function<std::optional<string>(const string&)>> op;
		//�û��Զ���Ĳ���������ֵ��bool���Ƿ��ö����е���һ������������ִ�У�string:׼�����ظ��ͻ��˵�string
		//������const string&:�����ӵ�object��key��
		watchInfo(size_t sessionID,
			watcherPtr nodes,
			std::shared_ptr<std::function<std::optional<string>(const string&)>> op) :
			sessionID(sessionID), nodes(nodes), op(op) {}
	};

	class objectMap
	{
	private:
		boost::container::list<keyInfo> keylist;
		hash_map<keyIter, object> map;
		std::set<keyIter> expireHeap;//Ŀǰ����ʵ�֣�û������ʹ��*��*
		//��Ҫֱ�Ӵ����������(ͨ��getObjectMap����ȡ��
		void _updateExpireTime(const keyIter& iter, keyInfo::time_duration ms);
	public:
		//����(����)һ��key
		void updateExpireTime(const keyIter& iter,std::chrono::seconds sec);
		void updateExpireTime(const keyIter& iter, std::chrono::milliseconds ms);
		void updateExpireTime(const keyIter& iter, std::chrono::steady_clock::time_point unix_ms_tp);
		static asio::awaitable<void> expiredKeyCollecting();
		void update(keyInfo&& key,object&& obj);
		hash_map<keyIter, object>::iterator try_insert(keyInfo&& key, object&& obj);
		hash_map<keyIter, object>::iterator find(const string& str);
		hash_map<keyIter, object>::iterator find(string&& str);
		hash_map<keyIter, object>::iterator begin();
		hash_map<keyIter, object>::iterator end();
		boost::container::list<keyInfo>::iterator keyBegin();
		boost::container::list<keyInfo>::iterator keyEnd();
		size_t erase(string&& str);
		hash_map<keyIter, object>::iterator erase(hash_map<keyIter, object>::const_iterator iter);
		static objectMap& getObjectMap(size_t index);
		static watchMap& getWatchMap(size_t index);
		size_t size() const;
		template<typename Iter>
		static void addWatches(Iter begin, Iter end, size_t dataBaseID,size_t sessionID,watcherPtr& tab,
							   std::function<std::optional<string>(const string&)>&& op)
		{
			auto &map = objectMap::getWatchMap(dataBaseID);
			auto func = std::make_shared<std::function<std::optional<string>(const string&)>>(std::move(op));
			for (; begin != end; begin = std::next(begin))//�������е�key
			{
				auto iter = map.find(*begin);
				if (iter == map.end())//������ӱ��в��������key�������һ��
				{
					iter = map.emplace(*begin, boost::container::list<watchInfo>()).first;
				}
				if (tab->find(&iter->second) != tab->end())//����Ѿ�������ͬ��key��ֱ������
					continue;
				iter->second.emplace_back(watchInfo(sessionID, tab, func));//�����key�ļ��Ӷ��еĶ�β���һ��������
				tab->emplace(&iter->second, std::prev(iter->second.end()));//��
			}
		}
	};

	
}
