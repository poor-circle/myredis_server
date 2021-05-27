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
		std::deque<std::vector<string>> cmdQueue;
		std::deque<asio::steady_timer*> clkQueue;
		std::unique_ptr<asio::ip::tcp::socket> soc;
		serverCas cas;
		asio::steady_timer clk,sendClk;
		static std::unique_ptr<SonServer>& _getSonServer();
		uint16_t sonServerNormalPort;
	public:
		asio::awaitable<void> run();
		void closeConnect();
		asio::awaitable<void> send(std::vector<string>&& cmd,std::chrono::seconds);
		serverCas getCas();
		static SonServer* getSonServer();
		static void rawSonServer(asio::io_context& ioc, uint16_t port);
		asio::awaitable<void> sendCmd(std::vector<string>&& cmd);
		SonServer(asio::io_context& ioc, uint16_t port);
		asio::ip::tcp::endpoint getAddress();
	};
}