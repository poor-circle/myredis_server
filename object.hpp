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

	struct watchInfo
	{
		size_t sessionID;//�߳�ID
		std::shared_ptr<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>> nodes;
		//��¼����key�ϵļ�����
		std::shared_ptr<std::function<std::optional<string>(const string&)>> op;
		//�û��Զ���Ĳ���������ֵ��bool���Ƿ��ö����е���һ������������ִ�У�string:׼�����ظ��ͻ��˵�string
		//������const string&:�����ӵ�object��key��
		watchInfo(size_t sessionID,
			std::shared_ptr<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>> nodes,
			std::shared_ptr<std::function<std::optional<string>(const string&)>> op) :
			sessionID(sessionID), nodes(nodes), op(op) {}
	};

	

	class objectMap
	{
	private:
		boost::container::list<string> keylist;
		hash_map<keyIter, object> map;
		//��Ҫֱ�Ӵ����������(ͨ��getObjectMap����ȡ��
	public:
		//����(����)һ��key
		void update(string&& str,object&& obj);
		hash_map<keyIter, object>::iterator try_insert(string&& str, object&& obj);
		hash_map<keyIter, object>::iterator find(const string& str);
		hash_map<keyIter, object>::iterator find(string&& str);
		hash_map<keyIter, object>::iterator begin();
		hash_map<keyIter, object>::iterator end();
		boost::container::list<string>::iterator keyBegin();
		boost::container::list<string>::iterator keyEnd();
		size_t erase(string&& str);
		hash_map<keyIter, object>::iterator erase(hash_map<keyIter, object>::const_iterator iter);
		static objectMap& getObjectMap(size_t index);
		static watchMap& getWatchMap(size_t index);
		size_t size() const;
		template<typename Iter>
		static auto addWatches(Iter begin, Iter end, size_t dataBaseID,size_t sessionID, std::function<std::optional<string>(const string&)>&& op)
		{
			auto &map = objectMap::getWatchMap(dataBaseID);
			auto tab = std::make_shared
							<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>>();
			auto func = std::make_shared<std::function<std::optional<string>(const string&)>>(std::move(op));
			for (; begin != end; begin = std::next(begin))
			{
				auto iter = map.find(*begin);
				if (iter == map.end())
				{
					iter = map.emplace(*begin, boost::container::list<watchInfo>()).first;
				}
				iter->second.emplace_back(watchInfo(sessionID, tab, func));
				auto temp = std::prev(iter->second.end());
				auto temp2 = iter->second.begin();
				tab->emplace(&iter->second, temp);
			}
			return tab;
		}
	};

	
}
