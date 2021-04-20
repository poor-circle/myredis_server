#include "stdafx.h"
#include "func.h"
#include "stringFunc.h"
#include "../object.hpp"
#include "../ObjectVisitor/StringVisitor/get.h"
#include "../ObjectVisitor/StringVisitor/changeCodetoString.h"
#include "../ObjectVisitor/StringVisitor/incr.h"


//func���൱��spring boot��controller��
//ͨ��controller����visitor��
//���ø�ʽ��visit([](auto& e) {return visitor::funcName(e); }, object)
//visitor::funcName����Ҫ���õĺ�����
//object����Ҫ�����Ķ�������
namespace myredis::func
{
#include"../namespace.i"

	//����һ���ַ���

	//��������ÿһ������Ӧ�þ����ܱ�֤����������

	//noexcept:ÿһ����������֤�����׳��κ��쳣����ʹ��һ������������Ҳ�ܱ�֤��Ӱ����������

	optional<string> set(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			getObjectMap().update(std::move(args[1]), stringToObject(std::move(args[2])));
			return code::succees_reply;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	//��ȡһ���ַ���
	optional<string> get(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.second);
				}
				return code::getBulkReply(ret.second);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}",e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* @author:tigerwang
	* date:2021/4/18
	* append ���string����
	* ��� key �Ѿ����ڣ�����ֵΪ�ַ�������ô��������� value ׷�ӵ�ԭ��ֵ��value���Ľ�β��������string�ĳ��ȡ� 
	* ��� key �����ڣ���ô�������ȴ���һ�����ַ�����key����ִ��׷�Ӳ������������ APPEND �������� SET ������
	* created by tigerwang  2021/4/17 20:00
	*/
	std::optional<string> append(std::vector<string>&& args) noexcept 
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				auto len = args[2].size();
				// ����set����
				getObjectMap().update(std::move(args[1]), stringToObject(std::move(args[2])));
				//set�Ժ�Ҫ���س���
				return code::getIntegerReply(len);
			}
			else
			{
				//���������ǿ��ת��Ϊ�ַ���
				auto ret = visit([iter](auto& e)
				{
					return visitor::changeCodetoString(e,iter->second);
				}, iter->second);
			
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.second);
				}
				// append����
				ret.second.append(args[2]);
				return code::getIntegerReply(ret.second.size());
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/* 
	* @author:tigerwang
	* date:2021/4/18
	* strlen ���string���͵Ķ���
	* ����key��string����value�ĳ��ȡ�
	* ���key��Ӧ�ķ�string���ͣ��ͷ��ش���
	* ���key�����ڷ���0
	*/
	std::optional<string> strlen(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())
			{
				//�Ҳ�����Ӧ��key ����0
				return code::getIntegerReply(0);
			}
			else
			{
				// �ҵ�keyȡ��
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success) {
					return code::getErrorReply(ret.second);
				}
				return code::getIntegerReply(ret.second.size());
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* @author:tigerwang
	* date:2021/4/18
	* usage: GETRANGE key start end
	* ���棺��������Ǳ��ĳ�GETRANGE�ģ���С��2.0��Redis�汾�н�SUBSTR��
	* ����key��Ӧ���ַ���value���Ӵ�������Ӵ�����start��endλ�ƾ����ģ����߶���string�ڣ�
	* �����ø���λ������ʾ��stringβ����ʼ�����±ꡣ����-1�������һ���ַ���-2���ǵ����ڶ������Դ����ơ�
	* ���������������Χ������ʱ�����ѽ��������string�ڡ�
	* ����ֵ bulk-reply
	*/
	std::optional<string> getrange(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())
			{
				//�Ҳ�����Ӧ��key
				return code::nil;
			}
			else
			{
				// �ҵ�keyȡ��string
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success) {
					return std::move(code::getErrorReply(ret.second));
				}
				// ʹ��stoi��start��endת��ΪInt
				// ����ת�����׳�invalid_argument�쳣
				// �������ػ��׳�out_of_range�쳣
				int64_t start, end;
				if (try_lexical_convert(args[2], start) == false || 
					try_lexical_convert(args[3], end) == false) {
					return code::getErrorReply("error:invalid_arugment or integer out of range");
				}
				const int64_t strLen = ret.second.size();
				// ���С���� + strLen ����С��0˵�����������ˣ�ֱ����0
				if (start < 0) 
					start = strLen + start;
				if (start < 0) 
					start = 0;
				if (end < 0)
					end = strLen + end;
				if (end < 0)
					end = 0;
				
				int64_t len = end - start + 1;
				if (start + len > strLen) 
				{
					len = strLen - start;
				}
				if (start > strLen || len < 0)
				{
					return code::getBulkReply("");
				}
				return code::getBulkReply(string_view(ret.second.c_str()+ start,len));
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* @author:tigerwang
	* date:2021/4/18
	* ��key����ֵΪvalue��
	* ���key�����ڣ���������µ�ͬSET��� 
	* ��key����ʱ��ʲôҲ������
	* SETNX�ǡ�SET if Not eXists���ļ�д��
	* 
	* ����ֵ�� Integer Reply
	* 1 ���key��������
	* 0 ���keyû�б�����
	*/
	std::optional<string> setnx(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter=getObjectMap().try_insert(std::move(args[1]), stringToObject(std::move(args[2])));
			return code::getIntegerReply(iter != getObjectMap().end()?1:0);
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* @author:tigerwang
	* date:2021/4/18
	* �Զ���key��Ӧ��value���ҷ���ԭ��key��Ӧ��value��
	* ���key���ڵ��Ƕ�Ӧ��value�����ַ������ͷ��ش���
	* ����ֵ�� bulk-string-reply
	* ����֮ǰ�ľ�ֵ�����֮ǰKey�����ڽ�����nil��
	* created by tigerwang	date:2021/4/18
	*/
	std::optional<string> getset(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 3)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end())//�Ҳ�����Ӧ��key
			{
				// ����nil
				return code::nil;
			}
			else
			{
				auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
				if (ret.first != code::status::success)
				{
					return code::getErrorReply(ret.second);
				}
				// ��ȡ֮ǰ��ֵ
				auto retstr = code::getBulkReply(ret.second);

				// ����
				iter->second = stringToObject(std::move(args[2]));

				// ����֮ǰ��ֵ
				return retstr;
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
	
	/*
	* @author: tigerwang 
	* date:2021/4/19
	* msetnx 
	* ��Ӧ������keys��������Ӧ��values�ϡ�ֻҪ��һ��key�Ѿ����ڣ�MSETNXһ������������ִ�С�
	* �����������ԣ�MSETNX����ʵ��Ҫô���еĲ������ɹ���Ҫôһ������ִ�У�
	* ���������������ò�ͬ��key������ʾһ��Ψһ�Ķ���Ĳ�ͬ�ֶΡ�
	* MSETNX��ԭ�ӵģ��������и�����keys��һ����set�ġ�
	* �ͻ��˲����ܿ�������һ����keys�����¶������û�иı�������
	* 
	* ����ֵ
	* integer-reply��ֻ����������ֵ��
	* 1 ������е�key��set
	* 0 ���û��key��set(����������һ��key�Ǵ��ڵ�)
	*/
	std::optional<string> msetnx(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() > 1 && args.size() % 2 == 0)
				return code::args_count_error;
			for (auto arg = args.begin() + 1; arg < args.end(); arg += 2) 
			{
				auto iter = getObjectMap().find(*arg);
				if (iter != getObjectMap().end()) 
				{
					return code::getIntegerReply(0);
				}
			}
			for (auto arg = args.begin() + 1; arg < args.end(); arg += 2)
			{

				getObjectMap().update(std::move(*arg), stringToObject(std::move(*(arg + 1))));
			}
			return code::getIntegerReply(1);
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* @author: tigerewang
	* date:2021/4/19
	* incr
	* �Դ洢��ָ��key����ִֵ��ԭ�ӵļ�1������
	* ���ָ����key�����ڣ���ô��ִ��incr����֮ǰ�����Ƚ�����ֵ�趨Ϊ0��
	* ���ָ����key�д洢��ֵ�����ַ������ͻ��ߴ洢���ַ������Ͳ��ܱ�ʾΪһ��������
	* ��ôִ���������ʱ�������᷵��һ������( ERR value is not an integer or out of range)��
	* �������������64λ���з����������ݡ�
	* ע��: ����redis��û��һ����ȷ����������ʾ�������ݣ��������������һ���ַ���������
	* ִ�����������ʱ��key��Ӧ�洢���ַ���������Ϊ10���Ƶ�64λ�з����������ݡ�
	* 
	* ����ֵ
	* integer-reply:ִ�е���������key��Ӧ��ֵ��
	*/
	std::optional<string> incr(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 2)
				return code::args_count_error;
			auto iter = getObjectMap().find(args[1]);
			if (iter == getObjectMap().end()) {
				getObjectMap().update(std::move(args[1]), stringToObject("1"));
				return code::getIntegerReply(1);
			}
			else {
				// ��visit��+1
				auto ret = visit([](auto& e)
				{
					return visitor::incr(e);
				}, iter->second);

				if (ret.first != code::status::success) {
					if (ret.second == INT64_MAX) {
						return code::getErrorReply("error: increment would overflow");
					}
					return code::getErrorReply("error:value is not an integer or out of range");
				}

				return code::getIntegerReply(ret.second);
			}
			return code::succees_reply;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	* 
	* @author: Lizezheng
	* mset:����һ�������ö����ֵ
	* date:2021/04/18
	* 
	*/
	std::optional<string> mset(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size()>1 && args.size()%2==0)
				return code::args_count_error;
			for (auto arg = args.begin() + 1; arg < args.end(); arg += 2)
			{
				getObjectMap().update(std::move(*arg), stringToObject(std::move(*(arg+1))));
			}
			return code::succees_reply;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}

	/*
	*
	* @author: Lizezheng
	* mget:����һ���Ի�ȡ�����ֵ
	* date:2021/04/18
	*
	*/
	std::optional<string> mget(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
				return code::getMultiReply(args.begin() + 1, args.end(),
				[](vector<string>::iterator &arg)
				{
					auto iter = getObjectMap().find(*arg);
					if (iter == getObjectMap().end())
					{
						return code::nil;
					}
					else
					{
						auto ret = visit([](auto& e) {return visitor::get(e); }, iter->second);
						if (ret.first != code::status::success)
							return code::nil;
						else
							return code::getBulkReply(ret.second);
					}
				});
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
	
	/*
	*
	* @author: Lizezheng
	* setrange:��һ��string�����丳ֵ
	* ���ַ���û����ô�����򲻴��ڣ�ʱ����ǰ�油0
	* date:2021/04/18
	*/
	std::optional<string> setrange(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() != 4)
				return code::args_count_error;
			int64_t offset;
			if (!try_lexical_convert<int64_t>(args[2], offset)||offset<0)
				return code::getErrorReply("invalid argument");
			else
			{
				auto iter = getObjectMap().find(args[1]);
				size_t retlength;
				if (iter != getObjectMap().end())
				{
					//�������������Ϊ�ַ�����setrange�Ժ󣬶���һ������ַ�����
					auto ret = visit([&iter](auto& e)
					{
						return visitor::changeCodetoString(e, iter->second);
					}, iter->second);
					if (ret.first != code::status::success)
					{
						return code::getErrorReply(ret.second);
					}
					else
					{
						string& str = ret.second;
						str.reserve(offset + args[3].size());//Ԥ����ռ�
						if (str.length() >= offset)//����ַ�����offset��
						{
							if (str.length() < offset + args[3].size())//����ַ���������
							{
								str.append(offset + args[3].size()-str.length(), '\0');//����
							}
							copy(args[3].begin(), args[3].end(), str.begin() + offset);
						}
						else
						{
							str.append(offset-str.length(), '\0');//����
							str.append(args[3]);//�����ַ���
						}
						retlength = str.length();
					}
				}
				else
				{
					auto ptr = make_unique<string>();
					string& str = *ptr;//��ȡָ����ָ���ַ���
					//����һ���ַ�������
					auto iter=getObjectMap().try_insert(std::move(args[1]), std::move(ptr));
					str.reserve(offset + args[3].size());//Ԥ����ռ�
					str.append(offset, '\0');//����
					str.append(args[3]);//�����ַ���
					retlength = str.length();
				}
				return code::getIntegerReply(retlength);
			}
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}


	std::optional<string> ping(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() == 1)
				return code::pong;
			else if (args.size() == 2)
				return code::getSingleReply(args[1]);
			return code::args_count_error;
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}


}