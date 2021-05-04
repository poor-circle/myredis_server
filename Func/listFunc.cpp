#include "stdafx.h"
#include "func.h"
#include "listFunc.h"
#include "../object.hpp"
#include "../code.h"
#include "../ObjectVisitor/ListVisitor/push.h"
#include "../ObjectVisitor/ListVisitor/llen.h"
#include "../ObjectVisitor/ListVisitor/lrange.h"
#include "../ObjectVisitor/ListVisitor/pop.h"
#include "../ObjectVisitor/ListVisitor/rpoplpush.h"
#include "../ObjectVisitor/ListVisitor/lset.h"
#include "../ObjectVisitor/ListVisitor/lindex.h"
#include "../ObjectVisitor/ListVisitor/ltrim.h"
#include "../ObjectVisitor/ListVisitor/lrem.h"
#include "../ObjectVisitor/ListVisitor/linsert.h"


namespace myredis::func {
#include"../namespace.i"

	/*
	* lpush
	* @author:tigerwang 
	* date:2021/4/21
	*/
	std::optional<string> lpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			vector<string> pushContent;
			for (int i = 2; i < args.size(); i++) {
				pushContent.push_back(args[i]);
			}
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end()) 
			{
				// ���û�ҵ�,���½�һ�����б�,����ͷ������
				auto list = std::make_unique<deque<string>>();
				auto ret = visitor::lpush(list, pushContent, args[1],ctx.session);
				objectMap.update(std::move(args[1]), std::move(list));
				return code::getIntegerReply(ret.second);
			}
			else 
			{
				auto ret = visit([&pushContent,&ctx,&args](auto& e)
				{
					return visitor::lpush(e,pushContent,args[1], ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	
	/*
	* lpushx
	* @author:tigerwang 
	* date:2021/4/21
	*/
	std::optional<string> lpushx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			vector<string> pushContent;
			for (int i = 2; i < args.size(); i++) {
				pushContent.push_back(args[i]);
			}
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// ���û�ҵ�,ʲô������
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&pushContent,&args,&ctx](auto& e)
				{
					return visitor::lpush(e, pushContent,args[1],ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/* 
	* llen
	* @author:tigerwang
	* date:2021/4/21
	*/
	std::optional<string> llen(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// ���û�ҵ�,�����ǿ�list ����0
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::llen(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* lrange
	* @author:tigerwang
	* date:2021/4/22
	*/
	std::optional<string> lrange(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// �Ҳ�����Ӧ��key����nil
				return code::nil;
			}
			else
			{
				// start,end ����Ӧ��lrange������ִ��
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false ||
					try_lexical_convert(args[3], end) == false)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}

				auto ret = visit([start,end](auto& e)
				{
					return visitor::lrange(e, start, end);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				auto s= code::getMultiReplyByRange(ret.second.begin(), ret.second.end(),
					[&ctx](vector<string>::iterator arg, std::back_insert_iterator<string> s)
				{
					code::getBulkReplyTo(*arg,s);
					return 1;
				});
				return s;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* rpush
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> rpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			vector<string> pushContent;
			for (int i = 2; i < args.size(); i++) {
				pushContent.push_back(args[i]);
			}
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// ���û�ҵ�,���½�һ�����б�,����ͷ������
				auto list = std::make_unique<deque<string>>();
				auto ret = visitor::rpush(list, pushContent,args[1],ctx.session);
				objectMap.update(std::move(args[1]), std::move(list));
				return code::getIntegerReply(ret.second);
			}
			else
			{
				auto ret = visit([&pushContent,&args,&ctx](auto& e)
				{
					return visitor::rpush(e, pushContent,args[1],ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* rpushx
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> rpushx(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			vector<string> pushContent;
			for (int i = 2; i < args.size(); i++) {
				pushContent.push_back(args[i]);
			}
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// ���û�ҵ�,ʲô������
				return code::getIntegerReply(0);
			}
			else
			{
				auto ret = visit([&pushContent,&args,&ctx](auto& e)
				{
					return visitor::rpush(e, pushContent,args[1],ctx.session);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getIntegerReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* lpop
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> lpop(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::lpop(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::nil;
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* rpop
	* @author:tigerwang
	* date:2021/4/23
	*/
	std::optional<string> rpop(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e)
				{
					return visitor::rpop(e);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::nil;
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* rpoplpush source destination
	* @author:tigerwang 
	* date:2021/4/23
	* update:
	* 2021/4/30 ��rpoplpush��Ϊ��pop��push�ķ�ʽ. ������:��sourceΪ��list,destination������ʱ,���ʼ��destinationΪ���б�
	*/
	std::optional<string> rpoplpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			
			auto srcIter = objectMap.find(args[1]);

			if (srcIter == objectMap.end())
			{
				// source������
				return code::nil;
			}
			// ȷ��source�Ƿ�ʱlist
			auto srcLenRet = visit([](auto& e)
			{
				return visitor::llen(e);
			}, srcIter->second);

			// ����list ����error;
			if (srcLenRet.first != code::status::success)
			{
				return code::getErrorReply(srcLenRet.first);
			}

			auto destnIter = srcIter;
			if (args[1] != args[2])
			{
				// ���source��destination��ͬ,��һ��destination��Ӧ��
				destnIter = objectMap.find(args[2]);
				if (destnIter == objectMap.end()) {
					// �Ҳ���destination 
					object list = std::make_unique<deque<string>>();
					string destnKey = args[2];
					// �½�һ�����б�
					objectMap.update(std::move(destnKey), std::move(list));
				}
				else
				{
					// �ҵ�destination,ȷ���Ƿ���list
					auto lenRet = visit([](auto& e)
					{
						return visitor::llen(e);
					}, destnIter->second);
					// ����list ����error;
					if (lenRet.first != code::status::success)
					{
						return code::getErrorReply(lenRet.first);
					}
				}
			}

			// destination��source��ΪList
			auto popRet = visit([](auto& e)
			{
				return visitor::rpop(e);
			}, srcIter->second);

			// sourceΪ���б�
			if (popRet.first != code::status::success)
			{
				return code::nil;
			}

			// ��ȡpop������
			std::vector<string> pushContent;
			pushContent.emplace_back(popRet.second);
			string destnKey = args[2];
			// lpush 
			destnIter = objectMap.find(destnKey);
			auto pushRet = visit([&pushContent, &ctx, &destnKey](auto& e)
			{
				return visitor::lpush(e, pushContent, destnKey, ctx.session);
			}, destnIter->second);

			if (pushRet.first != code::status::success)
			{
				return code::nil;
			}
			return code::getBulkReply(popRet.second);
				
			
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	
	/*
	* lset key index value
	* @author:tigerwang
	* date:2021/4/25
	*/
	std::optional<string> lset(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::key_error;
			}
			else
			{
				// ��indexת��Ϊ����
				int64_t index;
				if (try_lexical_convert(args[2], index) == false
					)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				auto& value = args[3];
				auto ret = visit([index,&value](auto& e)
				{
					return visitor::lset(e,index, value);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::ok;
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* lindex key index
	* @author:tigerwang
	* date:2021/4/25
	*/
	std::optional<string> lindex(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::key_error;
			}
			else
			{
				// ��indexת��Ϊ����
				int64_t index;
				if (try_lexical_convert(args[2], index) == false
					)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				auto ret = visit([index](auto& e)
				{
					return visitor::lindex(e, index);
				}, iter->second);

				if (ret.first == code::status::index_out_of_range) {
					return code::nil;
				}
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* ltrim key start stop
	* @author:tigerwang
	* date:2021/4/25
	*/
	std::optional<string> ltrim(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// �Ҳ�����Ӧ��key����nil
				return code::key_error;
			}
			else
			{
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false ||
					try_lexical_convert(args[3], end) == false)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}

				auto ret = visit([start, end](auto& e)
				{
					return visitor::ltrim(e, start, end);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::ok;

			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* lrem key count value
	* @author:tigerwang
	* date:2021/4/26
	*/
	std::optional<string> lrem(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				// �Ҳ�����Ӧ��key����nil
				return code::getIntegerReply(0);
			}
			else
			{
				// start,end ����Ӧ��lrange������ִ��
				int64_t count;
				if (try_lexical_convert(args[2], count) == false )
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				auto& value = args[3];
				auto ret = visit([count, &value](auto& e)
				{
					return visitor::lrem(e, count, value);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);

			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* linsert key BEFORE|AFTER pivot value
	* @author:tigerwang
	* date:2021/4/26
	*/
	std::optional<string> linsert(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 5)
				return code::args_count_error;
			auto iter = objectMap.find(args[1]);
			if (iter == objectMap.end())
			{
				return code::key_error;
			}
			else
			{
				auto& insertType = args[2];
				boost::algorithm::to_lower(insertType);
				if (insertType != "before"sv && insertType!="after"sv)
				{
					return code::getErrorReply(code::status::invaild_argument);
				}
				
				auto& pivot = args[3];
				auto& value = args[4];
				auto ret = visit([&insertType,&pivot, &value](auto& e)
				{ 
					return visitor::linsert(e, insertType, pivot, value);
				}, iter->second);

				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.first);
				}

				return code::getIntegerReply(ret.second);

			}
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* blpop key [key...] timeout 
	* block and wait if list isnt empty
	* @author:lizezheng
	* date:2021/4/27
	*/
	std::optional<string> blpop(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() <3)
				return code::args_count_error;
			int64_t sec;
			if (!try_lexical_convert(args.back(), sec) || sec < 0)//�õ�����ʱ��
			{
				return code::args_illegal_error;
			}
			//��������Ѿ��п��Է��ص�string��ֱ�ӷ���
			for (auto arg = args.begin() + 1, endarg = args.end() - 1; arg != endarg; ++arg)
			{
				auto iter = objectMap.find(*arg);
				if (iter != objectMap.end())
				{
					auto ret = visit([](auto& e)
					{
						return visitor::lpop(e);
					}, iter->second);
					if (ret.first == code::status::success)
					{
						//����key�����ֺ�key��ֵ
						return code::getMultiReply(*arg, ret.second);
					}
					else if (ret.first == code::status::object_type_error)
					{
						return code::getErrorReply(code::status::object_type_error);
					}
				}
			}
			//����׼����������̬
			auto watch_list = std::make_shared<watcher>();
			//��Ӽ������������ں��ʵ�����»��ѻỰ
			objectMap::addWatches(args.begin() + 1, args.end() - 1,ctx.session.getDataBaseID(),ctx.session.getSessionID(), watch_list,
				[&objectMap](const string& keyName)
				{
					auto iter = objectMap.find(keyName);
					optional<string> ret=nullopt;//���ؿգ�����ʧ�ܣ�����������������
					//���طǿ�ֵ��������ӳɹ������̵߳����м���������ɾ���������ַ������ظ��ͻ��ˣ��˳�����״̬
					//�������Ϊ�գ����ؿգ��������ӡ�������в�Ϊ�գ��������еĶ��ף�ֹͣ���������䷵�ظ��ͻ���
					if (iter != objectMap.end())
					{
						auto ans = visit([](auto& e)
						{
							return visitor::lpop(e);
						}, iter->second);
						if (ans.first == code::status::success)
						{
							//����key�����ֺ�key��ֵ
							ret = code::getMultiReply(keyName, ans.second);
						}
					}
					return ret;
				});
			//���Ự��Ϊ����̬,����������ʱ��
			ctx.session.setBlocked(code::multi_nil,seconds(sec), watch_list);
			return nullopt;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* brpop key [key...] timeout
	* block and wait if list isnt empty
	* @author:tigerwang
	* date:2021/4/30
	*/
	std::optional<string> brpop(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() < 3)
				return code::args_count_error;
			int64_t sec;
			if (!try_lexical_convert(args.back(), sec) || sec < 0)//�õ�����ʱ��
			{
				return code::args_illegal_error;
			}
			//��������Ѿ��п��Է��ص�string��ֱ�ӷ���
			for (auto arg = args.begin() + 1, endarg = args.end() - 1; arg != endarg; ++arg)
			{
				auto iter = objectMap.find(*arg);
				if (iter != objectMap.end())
				{
					auto ret = visit([](auto& e)
					{
						return visitor::rpop(e);
					}, iter->second);
					if (ret.first == code::status::success)
					{
						//����key�����ֺ�key��ֵ
						return code::getMultiReply(*arg, ret.second);
					}
					else if (ret.first == code::status::object_type_error)
					{
						return code::getErrorReply(code::status::object_type_error);
					}
				}
			}
			//����׼����������̬
			auto watch_list = std::make_shared<watcher>();
			//��Ӽ������������ں��ʵ�����»��ѻỰ
			objectMap::addWatches(args.begin() + 1, args.end() - 1, ctx.session.getDataBaseID(), ctx.session.getSessionID(), watch_list,
				[&objectMap](const string& keyName)
			{
				auto iter = objectMap.find(keyName);
				optional<string> ret = nullopt;//���ؿգ�����ʧ�ܣ�����������������
				//���طǿ�ֵ��������ӳɹ������̵߳����м���������ɾ���������ַ������ظ��ͻ��ˣ��˳�����״̬
				//�������Ϊ�գ����ؿգ��������ӡ�������в�Ϊ�գ��������еĶ��ף�ֹͣ���������䷵�ظ��ͻ���
				if (iter != objectMap.end())
				{
					auto ans = visit([](auto& e)
					{
						return visitor::rpop(e);
					}, iter->second);
					if (ans.first == code::status::success)
					{
						//����key�����ֺ�key��ֵ
						ret = code::getMultiReply(keyName,ans.second,1);
					}
				}
				return ret;
			});
			//���Ự��Ϊ����̬,����������ʱ��
			ctx.session.setBlocked(code::multi_nil, seconds(sec), watch_list);
			return nullopt;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* brpoplpush source destination timeout 
	* block and wait if list isnt empty
	* @author:tigerwang
	* date:2021/4/30
	*/
	std::optional<string> brpoplpush(context&& ctx) noexcept
	{
		auto&& args = ctx.args;
		auto&& objectMap = ctx.session.getObjectMap();
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			int64_t sec;
			if (!try_lexical_convert(args.back(), sec) || sec < 0)//�õ�����ʱ��
			{
				return code::args_illegal_error;
			}
			auto srcIter = objectMap.find(args[1]);
			if (srcIter == objectMap.end())
			{
				// source������
				return code::nil;
			}

			// ��ȡsource�ĳ���,���source����Ԫ�أ���������rpoplpushһ����
			auto lenRet = visit([](auto& e)
			{
				return visitor::llen(e);
			}, srcIter->second);
			// source��Ϊlist
			if (lenRet.first != code::status::success)
			{
				return code::getErrorReply(lenRet.first);
			}

			auto destnIter = srcIter;
			if (args[1] != args[2])
			{
				// ���source��destination��ͬ,��һ��destination��Ӧ��value
				destnIter = objectMap.find(args[2]);
				if (destnIter == objectMap.end()) {
					// �Ҳ���destination 
					object list = std::make_unique<deque<string>>();
					string destnKey = args[2];
					// �½�һ�����б�
					objectMap.update(std::move(destnKey), std::move(list));
					// ����destnIter
					destnIter = objectMap.find(args[2]);
				}
				else
				{
					// �ҵ�destination,ȷ���Ƿ���list
					auto lenRet = visit([](auto& e)
					{
						return visitor::llen(e);
					}, destnIter->second);
					// ����list ����error;
					if (lenRet.first != code::status::success)
					{
						return code::getErrorReply(lenRet.first);
					}
				}
			}

			if(lenRet.second>0)
			{
				// destination��source��ΪList
				auto popRet = visit([](auto& e)
				{
					return visitor::rpop(e);
				}, srcIter->second);

				// ��ȡpop������
				std::vector<string> pushContent;
				pushContent.emplace_back(popRet.second);
				string destnKey = args[2];
				// lpush 

				auto pushRet = visit([&pushContent, &ctx, &destnKey](auto& e)
				{
					return visitor::lpush(e, pushContent, destnKey, ctx.session);
				}, destnIter->second);

				if (pushRet.first != code::status::success)
				{
					return code::nil;
				}
				return code::getBulkReply(popRet.second);
			}


			// brpoplpush ֻ��Ҫ����source�ǲ���Ϊ�ռ���
			//����׼����������̬
			auto watch_list = std::make_shared<watcher>();
			//��Ӽ������������ں��ʵ�����»��ѻỰ
			objectMap::addWatches(args.begin() + 1, args.begin() + 2 , ctx.session.getDataBaseID(), ctx.session.getSessionID(), watch_list,
				[&objectMap,&ctx,&args](const string& keyName)
			{
				auto iter = objectMap.find(keyName);

				optional<string> ret = nullopt;//���ؿգ�����ʧ�ܣ�����������������
				//���طǿ�ֵ��������ӳɹ������̵߳����м���������ɾ���������ַ������ظ��ͻ��ˣ��˳�����״̬
				//�������Ϊ�գ����ؿգ��������ӡ�������в�Ϊ�գ��������еĶ��ף�ֹͣ���������䷵�ظ��ͻ���
				if (iter != objectMap.end())
				{
					auto popRet = visit([](auto& e)
					{
						return visitor::rpop(e);
					}, iter->second);
					if (popRet.first == code::status::success)
					{
						//����key�����ֺ�key��ֵ
						std::vector<string> pushContent;
						pushContent.emplace_back(popRet.second);
						auto destnIter = objectMap.find(args[2]);
						auto pushRet = visit([&pushContent, &ctx, &args](auto& e)
						{
							return visitor::lpush(e, pushContent, args[2], ctx.session);
						}, destnIter->second);
						ret = code::getBulkReply(popRet.second);
					}
				}

				return ret;
			});
			//���Ự��Ϊ����̬,����������ʱ��
			ctx.session.setBlocked(code::multi_nil, seconds(sec), watch_list);
			return nullopt;
		}
		catch (const exception& e)
		{
			printlog(e);
			return nullopt;//���ؿ�ֵ
		}
	}
	
}