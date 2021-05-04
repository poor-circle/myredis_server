#include"stdafx.h"
#include"object.hpp"
#include "BaseSession.h"
namespace myredis
{
#include"namespace.i"

	objectMap& objectMap::getObjectMap(size_t index)
	{
		static vector<objectMap> map(data_base_count);
		return map[index];
	}
	watchMap& objectMap::getWatchMap(size_t index)
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
		_updateExpireTime(iter, duration_cast<milliseconds>((steady_clock::now() + ms).time_since_epoch()));
	}

	void objectMap::updateExpireTime(const keyIter& iter, steady_clock::time_point tp)
	{
		_updateExpireTime(iter, duration_cast<milliseconds>(tp.time_since_epoch()));
	}


	void objectMap::_updateExpireTime(const keyIter& iter, keyInfo::time_duration time)
	{
		if (iter.iter->isSetExpiredTime())
			expireHeap.erase(iter);
		iter.iter->liveTime = time;
		expireHeap.emplace(iter);
	}
	//����key��������������
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
				assert(map.map.size() == map.expireHeap.size());
				++cnt;
				auto now = steady_clock::now();
				if (cnt%10==0&&now > pre_time + long_work_time)//������������ʱ��
				{
					assert((fmt::print("then sleep:{},work:{}us\n\n", tp, (now- pre_time)/1us),1));
					cnt = 0;
					pre_index = i;
					sleepTime = max<double>(minsleepTime.count(),sleepTime/ downRate);
					tp = max(minsleepTime, min(milliseconds((int)sleepTime), stdsleepTime/2));//��û���յ�������˯��ʱ����루��ʹ�ÿ����������˯��ʱ����������ָ�����׼˯��ʱ�䣩
					clk.expires_after(tp);
					assert((fmt::print("least element:{}\n", map.expireHeap.size()), 1));
					assert(map.map.size() == map.expireHeap.size());
					assert((fmt::print("stop expiredKey scan\n"), 1));
					co_await clk.async_wait(asio::use_awaitable);//�ó�Э�̿���Ȩ
					assert((fmt::print("start expiredKey scan\n"), 1));
					pre_time = steady_clock::now();//��ʼ��һ�ֻ��գ������ʼʱ��
				}
			}
			assert((fmt::print("least element:{}\n",map.expireHeap.size()),1));
			assert(map.map.size() == map.expireHeap.size());
			i = (i + 1) % data_base_count;
			if (i == pre_index)//����һȦ�ˣ����е����������ոɾ���
			{
				assert((fmt::print("then sleep:{},work:{}us\n\n", tp, (steady_clock::now()-pre_time)/1us),1));
				sleepTime = min<double>(maxsleepTime.count(), sleepTime*riseRate);
				tp = min(maxsleepTime, milliseconds((int)sleepTime));//ûʲô������˯��ʱ��ӱ�
				clk.expires_after(tp);
				assert((fmt::print("stop expiredKey scan,then sleep:{}\n\n", tp), 1));
				co_await clk.async_wait(asio::use_awaitable);//�ó�Э�̿���Ȩ
				assert((fmt::print("start expiredKey scan\n"), 1));
				pre_time = steady_clock::now();//��ʼ��һ�ֻ��գ������ʼʱ��
			}
		} while (true);
	}
	//TODO::������Ҫ����Ƿ���Ҫ��̭ĳ������
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
			if (ans->first.iter->getLiveTime() != key.getLiveTime())//���¹���ʱ��
			{
				_updateExpireTime(ans->first.iter, key.getLiveTime());
			}
			ans->second = std::move(obj);
			keylist.pop_back();
		}
		assert(map.size() == expireHeap.size());
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

	//ÿ�β��Ҷ���Ҫ���»������ȼ�
	hash_map<keyIter, object>::iterator objectMap::find(const string& str)
	{
		string& temp = (string&)str;
		keylist.emplace_back(std::move(temp));
		auto ans = map.find(prev(keylist.end()));
		temp = std::move(keylist.back().str);
		keylist.pop_back();
		//���ڴ���
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

	//ÿ�β��Ҷ���Ҫ���»������ȼ�
	hash_map<keyIter, object>::iterator objectMap::find(string&& str)
	{
		keylist.emplace_back(std::move(str));
		auto ans = map.find(prev(keylist.end()));
		keylist.pop_back();
		//���ڴ���
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
		keylist.emplace_back(std::move(str));
		hash_map<keyIter, object>::const_iterator ans = map.find(prev(keylist.end()));
		keylist.pop_back();
		if (ans != map.cend())
		{
			map.erase(ans);
			auto iter = ans->first.iter;
			if (ans->first.iter->isSetExpiredTime())
				expireHeap.emplace(iter);
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

	boost::container::list<keyInfo>::iterator objectMap::keyBegin()
	{
		return keylist.begin();
	}
	boost::container::list<keyInfo>::iterator objectMap::keyEnd()
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