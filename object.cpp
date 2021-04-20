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
	//TODO::插入需要检查是否需要淘汰某个缓存
	void objectMap::update(string&& str, object&& obj)
	{
		keylist.push_front(std::move(str));
		auto ans = map.find(keylist.begin());
		if (ans==map.end())
		{
			map.emplace(std::move(keylist.begin()),std::move(obj));
		}
		else
		{
			ans->second = std::move(obj);
		}
		return;
	}

	//TODO::插入需要检查是否需要淘汰某个缓存
	hash_map<keyIter, object>::iterator objectMap::try_insert(string&& str, object&& obj)
	{
		keylist.push_front(std::move(str));
		auto ans = map.emplace(keylist.begin(), std::move(obj));
		if (!ans.second)
			keylist.pop_front();
		return ans.second?ans.first:map.end();
	}

	//每次查找都需要更新缓存优先级
	hash_map<keyIter, object>::iterator objectMap::find(string& str)
	{
		keylist.push_front(std::move(str));
		auto ans = map.find(keylist.begin());
		str = std::move(keylist.front());
		keylist.pop_front();
		if (allKeyLRU&&ans!=map.end())
		{
			keylist.push_front(std::move(*(ans->first.iter)));
			keylist.erase(ans->first.iter);
			ans->first.iter = keylist.begin();
		}
		return ans;
	}

	hash_map<keyIter, object>::iterator objectMap::end()
	{
		return map.end();
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