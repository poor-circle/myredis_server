#define _WIN32_WINNT 0x0A00 //设置操作系统
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING //忽略c++17废弃某些旧C标准库的警告
#define Regist(sessionType,value)\
	uint32_t sessionType::sessionTypeCode = Session::regist<sessionType, value>();
namespace myRedis
{
	constexpr int defaultPort = 8848;
}