#define _WIN32_WINNT 0x0A00 //���ò���ϵͳ
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING //����c++17����ĳЩ��C��׼��ľ���
#define Regist(sessionType,value)\
	uint32_t sessionType::sessionTypeCode = Session::regist<sessionType, value>();
namespace myRedis
{
	constexpr int defaultPort = 8848;
}