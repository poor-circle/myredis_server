#pragma once
#include"../stdafx.h"
namespace myredis
{
	class FatherServer
	{
	public:
		enum class serverCas
		{
			notConnect,//未连接
			rawing,//初始化中
			sync,//已同步
			no_father//无父服务器
		};
	private:
		asio::ip::tcp::socket soc;
		asio::io_context& ioc;
		serverCas cas;
		static std::unique_ptr<FatherServer>& _getFatherServer();
		uint16_t faServerNormalPort;
	public:
		asio::awaitable<void> sync(std::string_view address, uint16_t port);
		asio::awaitable<void> run();
		serverCas getCas();
		static void setFatherServer(asio::io_context &ioc);
		static FatherServer* getFatherServer();
		asio::ip::tcp::endpoint getAddress();
		FatherServer(asio::io_context& ioc);
	};
}