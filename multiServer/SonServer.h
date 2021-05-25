#pragma once
#include"../stdafx.h"
namespace myredis
{
	class SonServer
	{
	public:
		enum class serverCas
		{
			notConnect,//未连接
			rawing,//初始化中
			sync,//已同步
		};
	private:
		void setCas(serverCas cas);
		uint16_t listen_port;
		asio::io_context& ioc;
		std::queue<std::vector<string>> que;
		std::unique_ptr<asio::ip::tcp::socket> soc;
		serverCas cas;
		asio::steady_timer clk,timeout_clk;

		static std::unique_ptr<SonServer>& _getSonServer();
	public:
		SonServer(asio::io_context& ioc, uint16_t port);
		asio::awaitable<void> run();
		asio::awaitable<void> send(std::vector<string>& cmd,std::chrono::seconds);
		serverCas getCas();
		static SonServer& getSonServer();
		static void rawSonServer(asio::io_context& ioc, uint16_t port);
	};
}