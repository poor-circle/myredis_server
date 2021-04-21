#include"stdafx.h"
#include"object.hpp"
namespace myredis
{
#include"namespace.i"


	objectMap& objectMap::getObjectMap(size_t index)
	{
		static vector<objectMap> map(data_base_count);
		return map[index];
	}
	size_t objectMap::size() const
	{
		return map.size();
	}
	//TODO::插入需要检查是否需要淘汰某个缓存
	void objectMap::update(string&& str, object&& obj)
	{
		keylist.push_back(std::move(str));
		auto ans = map.find(prev(keylist.end()));
		if (ans==map.end())
		{
			map.emplace(prev(keylist.end()),std::move(obj));
		}
		else
		{
			ans->second = std::move(obj);
			keylist.pop_back();
		}
		return;
	}

	//TODO::插入需要检查是否需要淘汰某个缓存
	hash_map<keyIter, object>::iterator objectMap::try_insert(string&& str, object&& obj)
	{
		keylist.push_back(std::move(str));
		auto ans = map.emplace(prev(keylist.end()), std::move(obj));
		if (!ans.second)
			keylist.pop_back();
		return ans.second?ans.first:map.end();
	}

	//每次查找都需要更新缓存优先级
	hash_map<keyIter, object>::iterator objectMap::find(string& str)
	{
		keylist.push_back(std::move(str));
		auto ans = map.find(prev(keylist.end()));
		str = std::move(keylist.back());
		keylist.pop_back();
		return ans;
	}

	//每次查找都需要更新缓存优先级
	hash_map<keyIter, object>::iterator objectMap::find(string&& str)
	{
		keylist.push_back(std::move(str));
		auto ans = map.find(prev(keylist.end()));
		keylist.pop_back();
		return ans;
	}

	hash_map<keyIter, object>::iterator objectMap::begin()
	{
		return map.begin();
	}

	hash_map<keyIter, object>::iterator objectMap::end()
	{
		return map.end();
	}

	size_t objectMap::erase(string&& str)
	{
		keylist.push_back(std::move(str));
		hash_map<keyIter, object>::const_iterator ans = map.find(prev(keylist.end()));
		keylist.pop_back();
		if (ans != map.end())
		{
			keylist.erase(ans->first.iter);
			map.erase(ans);
			return 1;
		}
		else 
			return 0;
	}

	hash_map<keyIter, object>::iterator objectMap::erase(hash_map<keyIter, object>::const_iterator iter)
	{
		keylist.erase(iter->first.iter);
		return map.erase(iter);
	}

	boost::container::list<string>::iterator objectMap::keyBegin()
	{
		return keylist.begin();
	}
	boost::container::list<string>::iterator objectMap::keyEnd()
	{
		return keylist.end();
	}


	object stringToObject(string&& str)
	{
		string temp;
		if (str.size() > 20)//it must be string
			return make_unique<string>(std::move(str));
		int64_t integer;
		double db;
		for (auto &&e:str)
			if (!isdigit(e) && e != '-'&& e != '.')
				return make_unique<string>(std::move(str));
		if (try_lexical_convert(str, integer))
		{
			if (str.size() >= 2 && (str[0] == '0' || (str[0] == '-' && str[1] == '0')));
			else
			{
				assert((fmt::format_to(back_inserter(temp), FMT_COMPILE("{}"), integer), temp == str));
				return integer;
			}
			
		}
		else if (try_lexical_convert(str, db))
		{
			fmt::format_to(back_inserter(temp), FMT_COMPILE("{}"), db);
			if (temp == str) return db;
		}
		return make_unique<string>(std::move(str));
	}
}