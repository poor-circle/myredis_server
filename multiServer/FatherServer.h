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
		};
	private:
		asio::ip::tcp::socket soc;
		asio::io_context& ioc;
	public:
		FatherServer(asio::io_context& ioc);
		asio::awaitable<void> run(std::string_view address, uint16_t port);
	};
}