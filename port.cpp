#include"stdafx.h"
#include"port.h"
namespace myredis
{
	uint16_t& getDefaultPort()
	{
		static uint16_t defaultPort = 6379;
		return defaultPort;
	}
	uint16_t& getDefaultInnerPort()
	{
		static uint16_t InnerPort = 6380;
		return InnerPort;
	}
}