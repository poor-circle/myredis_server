#define _WIN32_WINNT 0x0A00 //���ò���ϵͳ
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS //�������þ���
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
	constexpr int son_server_expire_seconds = 10;//���ӷ������ͨѶ��ʱ
	constexpr int son_server_speed_lower_rate = 10;//�ӷ���������������
	constexpr int max_un_sync_limit = 1000;
}