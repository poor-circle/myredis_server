#define _WIN32_WINNT 0x0A00 //���ò���ϵͳ
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS //�������þ���

#define Regist(sessionType,value)\
	uint32_t sessionType::sessionTypeCode = Session::regist<sessionType, value>();

//variant support
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

namespace myredis
{
	constexpr int defaultPort = 6379;//Ĭ�϶˿ڣ�TODO:��Ϊ�������ļ��ж�ȡ
}