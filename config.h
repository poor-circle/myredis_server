#define _WIN32_WINNT 0x0A00 //���ò���ϵͳ
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS //�������þ���

#define Regist(sessionType,value)\
	uint32_t sessionType::sessionTypeCode = Session::regist<sessionType, value>();

 //#define QPSTEST


constexpr bool allKeyLRU = 1;

inline const char *myredis_password = "123456";

inline long long data_base_count = 16;

inline size_t regexOpLowerBound = 1000;

//variant support
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

namespace myredis
{
	constexpr int defaultPort = 6379;//Ĭ�϶˿ڣ�TODO:��Ϊ�������ļ��ж�ȡ
}