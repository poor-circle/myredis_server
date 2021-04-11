#pragma once
#include"stdc++.h"
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
	//ͨ��key��ȡ��������
	hash_map<string, object>& getObjectMap();

	//��һ��Stringת��ΪString���͵Ķ���
	object stringToObject(string&& str);

}
