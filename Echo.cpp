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
        //��������С�ݶ�Ϊ2000���ֽ�
        constexpr static int BUFSIZE = 1000;//��������С�ݶ�Ϊ4000���ֽ�
        //����myredis�������ӣ��ڴ�֮ǰ���ѽ����������ӣ�
        virtual awaitable<void> Run()
        {
            co_return;
         
        }
       
    };                                                                                                             
    Regist(Echo, 0);
}