#define _WIN32_WINNT 0x0A00 //设置操作系统
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS //忽略弃用警告
#define FMT_HEADER_ONLY
#define Regist(sessionType,value)\
	uint32_t sessionType::sessionTypeCode = Session::regist<sessionType, value>();

//#define QPSTEST

//#define FFLUSH_EACH_COMMAND

constexpr bool allKeyLRU = 1;

inline const char *myredis_password = "";

inline long long data_base_count = 16;

inline size_t regexOpLowerBound = 1000;

inline size_t default_scan_count = 200;


//variant support
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

namespace myredis
{
	constexpr int defaultPort = 6379;//默认端口，TODO:改为从配置文件中读取
}