#include "stdafx.h"
#include "func.h"
#include "keyFunc.h"
#include "../object.hpp"
#include "../code.h"

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
	/*
	* @author: tigerwang
	* date:2021/4/19
	* exists key [key ...]
	* ����key�Ƿ���ڡ�
	* ����ֵ
	* integer-reply�����µ��������
	* 1 ���key����
	* 0 ���key������
	*/
	std::optional<string> exists(std::vector<string>&& args) noexcept
	{
		try
		{
			if (args.size() <= 1)
				return code::args_count_error;
			else
				return code::getMultiReply(args.begin() + 1, args.end(),
					[](vector<string>::iterator& arg)
			{
				auto iter = getObjectMap().find(*arg);
				if (iter == getObjectMap().end())
				{
					return code::getIntegerReply(0);
				}
				else
				{
					return code::getIntegerReply(1);
				}
			});
		}
		catch (const exception& e)
		{
			fmt::print("exception error:{}", e.what());
			return nullopt;//���ؿ�ֵ
		}
	}
}