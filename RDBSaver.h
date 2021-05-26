#pragma once
#include"stdafx.h"
namespace myredis
{
	class RDBSaver
	{
	private:
		static asio::awaitable<void> saveDBDetail(asio::io_context& ioc);
		static std::chrono::seconds& getDurationTime();
	public:
		static asio::awaitable<void> saveDBToDesk(asio::io_context& ioc, string& data);
		static void saveDB(asio::io_context& ioc);
		static void loadDB();
		static void setDurationTime(std::chrono::seconds time_duration);
	};


}