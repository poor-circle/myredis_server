#define _WIN32_WINNT 0x0A00 //设置操作系统
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS //忽略弃用警告
#define FMT_HEADER_ONLY
#define Regist(sessionType,value)\
	uint32_t sessionType::sessionTypeCode = Session::regist<sessionType, value>();

//#define QPSTEST

constexpr bool ENABLE_AOF = true;

#define FFLUSH_EACH_COMMAND

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

	constexpr bool enable_backup_server=true;
	constexpr int son_server_expire_seconds = 10;//父子服务器最长通讯延时
	constexpr int son_server_speed_lower_rate = 10;//子服务器的性能下限
	constexpr int max_un_sync_limit = 1000;
}