#include "stdafx.h"
#include "Session.h"
#include "object.hpp"
#include "Func/funcManager.h"
namespace myredis
{



    
#include"namespace.i"


#ifdef QPSTEST
    uint64_t cnter = 0;
    high_resolution_clock::time_point tp;
#endif
    class Echo :Session
    {
    public:
        static uint32_t sessionTypeCode;
        Echo(asio::io_context& ioc, tcp::socket socket) :Session(ioc, std::move(socket)) {};
        //缓冲区大小暂定为2000个字节
        constexpr static int BUFSIZE = 1000;//缓冲区大小暂定为4000个字节
        //处理myredis网络连接（在此之前，已建立网络连接）
        virtual awaitable<void> Run()
        {
            co_return;
         
        }
       
    };                                                                                                             
    Regist(Echo, 0);
}