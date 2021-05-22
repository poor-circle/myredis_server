#pragma once
#include"stdafx.h"
namespace myredis
{
	class RDBSaver
	{
	private:
		static asio::awaitable<void> saveDBDetail(asio::steady_timer& clk, asio::io_context& ioc);
		static std::chrono::seconds& getDurationTime();
	public:
		static void saveDB(asio::io_context& ioc);
		static void loadDB(asio::io_context& ioc);
		static void setDurationTime(std::chrono::seconds time_duration);
	};


}