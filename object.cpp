#include"stdafx.h"
#include"object.hpp"
#include "BaseSession.h"
namespace myredis
{
#include"namespace.i"

	objectMap& objectMap::getObjectMap(int64_t index)
	{
		static vector<objectMap> map(data_base_count);
		return map[index];
	}
	watchMap& objectMap::getWatchMap(int64_t index)
	{
		static vector<watchMap> map(data_base_count);
		return map[index];
	}
	size_t objectMap::size() const
	{
		return map.size();
	}

	void objectMap::updateExpireTime(const keyIter& iter, seconds sec)
	{
		return updateExpireTime(iter, duration_cast<milliseconds>(sec));
	}

	void objectMap::updateExpireTime(const keyIter& iter, milliseconds ms)
	{
		_updateExpireTime(iter, keyInfo::getUnixMSDuration(ms));
	}

	void objectMap::cancelExpireTime(const keyIter& iter)
	{
		_updateExpireTime(iter, milliseconds::duration::max());
	}

	void objectMap::updateExpireTime(const keyIter& iter, int64_t unix_ms_tp)
	{
		_updateExpireTime(iter, std::chrono::milliseconds(unix_ms_tp));
	}

	void objectMap::_updateExpireTime(const keyIter& iter, keyInfo::time_duration time)
	{
		if (iter.iter->isSetExpiredTime())
			expireHeap.erase(iter);
		iter.iter->liveTime = time;
		if (iter.iter->isSetExpiredTime())
			expireHeap.emplace(iter);
	}

	//过期key的主动垃圾回收
	asio::awaitable<void> objectMap::expiredKeyCollecting()
	{
		assert((fmt::print("start expiredKey scan\n"), 1));
		auto clk = asio::steady_timer(co_await asio::this_coro::executor);
		unsigned i = 0, cnt = 0,pre_index=0;
		auto pre_time = steady_clock::now();
		auto stdsleepTime=1000ms,maxsleepTime=3000ms,minsleepTime=1ms,long_work_time=1ms,tp= stdsleepTime;
		double sleepTime = 1000.0;
		const double riseRate = 1.2, downRate = 1.5;//ms
		do
		{
			auto& map = objectMap::getObjectMap(i);
			while (!map.expireHeap.empty() && map.expireHeap.begin()->iter->isExpired())
			{
				assert((fmt::print("erase expired key:{}\n",map.expireHeap.begin()->iter->getStr()), 1));
				map.map.erase(map.expireHeap.begin()->iter);
				map.keylist.erase(map.expireHeap.begin()->iter);
				map.expireHeap.erase(map.expireHeap.begin());
				assert(map.size() == map.expireHeap.size());
				++cnt;
				auto now = steady_clock::now();
				if (cnt%10==0&&now > pre_time + long_work_time)//垃圾回收上限时间
				{
					assert((fmt::print("then sleep:{},work:{}us\n\n", tp, (now- pre_time)/1us),1));
					cnt = 0;
					pre_index = i;
					sleepTime = max<double>(minsleepTime.count(),sleepTime/ downRate);
					tp = max(minsleepTime, min(milliseconds((int)sleepTime), stdsleepTime/2));//有没回收的垃圾，睡眠时间减半（且使用快启动，如果睡眠时间过长立即恢复到标准睡眠时间）
					clk.expires_after(tp);
					assert((fmt::print("least element:{}\n", map.expireHeap.size()), 1));
					//assert(map.map.size() == map.expireHeap.size());
					assert((fmt::print("stop expiredKey scan\n"), 1));
					co_await clk.async_wait(asio::use_awaitable);//让出协程控制权
					assert((fmt::print("start expiredKey scan\n"), 1));
					pre_time = steady_clock::now();//开始新一轮回收，重设初始时间
				}
			}
			assert((fmt::print("least element:{}\n",map.expireHeap.size()),1));
			//assert(map.map.size() == map.expireHeap.size());
			i = (i + 1) % data_base_count;
			if (i == pre_index)//绕了一圈了，所有的垃圾都回收干净了
			{
				assert((fmt::print("then sleep:{},work:{}us\n\n", tp, (steady_clock::now()-pre_time)/1us),1));
				sleepTime = min<double>(maxsleepTime.count(), sleepTime*riseRate);
				tp = min(maxsleepTime, milliseconds((int)sleepTime));//没什么垃圾，睡眠时间加倍
				clk.expires_after(tp);
				assert((fmt::print("stop expiredKey scan,then sleep:{}\n\n", tp), 1));
				co_await clk.async_wait(asio::use_awaitable);//让出协程控制权
				assert((fmt::print("start expiredKey scan\n"), 1));
				pre_time = steady_clock::now();//开始新一轮回收，重设初始时间
			}
		} while (true);
	}

	//TODO::插入需要检查是否需要淘汰某个缓存
	void objectMap::update(keyInfo&& key, object&& obj)
	{
		keylist.emplace_back(std::move(key));
		auto ans = map.find(prev(keylist.end()));
		if (ans==map.end())
		{
			auto iter = prev(keylist.end());
			map.emplace(iter,std::move(obj));
			if (keylist.back().isSetExpiredTime())
			{
				expireHeap.emplace(iter);
			}
		}
		else
		{
			if (ans->first.iter->getLiveTime() != key.getLiveTime())//更新过期时间
			{
				_updateExpireTime(ans->first.iter, key.getLiveTime());
			}
			ans->second = std::move(obj);
			keylist.pop_back();
		}
		//assert(map.size() == expireHeap.size());
		return;
	}

	hash_map<keyIter, object>::iterator objectMap::try_insert(keyInfo&& key, object&& obj)
	{
		keylist.emplace_back(std::move(key));
		auto iter = prev(keylist.end());
		auto ans = map.emplace(iter, std::move(obj));
		if (!ans.second)
			keylist.pop_back();
		else
		{
			if (iter->isSetExpiredTime())
				expireHeap.emplace(iter);
		}
		return ans.second?ans.first:map.end();
	}

	//每次查找都需要更新缓存优先级
	hash_map<keyIter, object>::iterator objectMap::find(const string& str)
	{
		string& temp = (string&)str;
		keylist.emplace_back(std::move(temp));
		auto ans = map.find(prev(keylist.end()));
		temp = std::move(keylist.back().str);
		keylist.pop_back();
		//过期处理
		if (ans!=map.end() && ans->first.iter->isExpired())
		{
			assert((fmt::print("erase expired key:{}\n", ans->first.iter->getStr()), 1));
			auto iter = ans->first.iter;
			map.erase((hash_map<keyIter, object>::const_iterator)ans);
			expireHeap.erase(iter);
			keylist.erase(iter);
			return map.end();
		}
		else
			return ans;
	}

	//每次查找都需要更新缓存优先级
	hash_map<keyIter, object>::iterator objectMap::find(string&& str)
	{
		keylist.emplace_back(std::move(str));
		auto ans = map.find(prev(keylist.end()));
		keylist.pop_back();
		//过期处理
		if (ans != map.end() && ans->first.iter->isExpired())
		{
			assert((fmt::print("erase expired key:{}\n", ans->first.iter->getStr()), 1));
			auto iter = ans->first.iter;
			map.erase((hash_map<keyIter, object>::const_iterator)ans);
			expireHeap.erase(iter);
			keylist.erase(iter);
			return map.end();
		}
		else
			return ans;
	}

	//从const key的iter获取const value,不淘汰缓存
	const object& objectMap::find(boost::container::list<keyInfo>::const_iterator iter)
	{
		return map.find(*(boost::container::list<keyInfo>::iterator*)(&iter))->second;
	}

	//获取const key的iter，不淘汰缓存
	hash_map<keyIter, object>::const_iterator objectMap::cbegin() const
	{
		return map.cbegin();
	}

	//获取const key的iter，不淘汰缓存
	hash_map<keyIter, object>::const_iterator objectMap::cend() const
	{
		return map.end();
	}

	size_t objectMap::erase(string&& str)
	{
		keylist.emplace_back(std::move(str));
		hash_map<keyIter, object>::const_iterator ans = map.find(prev(keylist.end()));
		keylist.pop_back();
		if (ans != map.cend())
		{
			auto iter = ans->first.iter;
			if (ans->first.iter->isSetExpiredTime())
				expireHeap.erase(iter);
			map.erase(ans);
			keylist.erase(iter);
			return 1;
		}
		else 
			return 0;
	}

	hash_map<keyIter, object>::iterator objectMap::erase(hash_map<keyIter, object>::const_iterator iter)
	{
		if (iter != map.cend())
		{
			auto keyIter = iter->first.iter;
			auto ret = map.erase(iter);
			if (iter->first.iter->isSetExpiredTime())
				expireHeap.emplace(keyIter);
			keylist.erase(keyIter);
			return ret;
		}
		else
			return map.end();
	}

	boost::container::list<keyInfo>::const_iterator objectMap::keyBegin() const
	{
		return keylist.cbegin();
	}
	boost::container::list<keyInfo>::const_iterator objectMap::keyEnd() const
	{
		return keylist.cend();
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

	

	void objectMap::reserve(size_t sz)
	{
		map.reserve(sz);
	}
}