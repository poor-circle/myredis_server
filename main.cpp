
#include "stdafx.h"
#include "Listener.h"
#include "benchmark.h"
#include "ObjectVisitor/serialize.h"
#include "RDBSaver.h"

#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


namespace myredis
{
    #include "namespace.i"
    static void run()
    {
        //Test();//debugʱִ�е�Ԫ����
        try
        {
            asio::io_context io_context;
            asio::signal_set signals(io_context,SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto) 
            { 
                io_context.stop();
            });
            myredis::Listener listener(io_context, myredis::defaultPort);
            RDBSaver::loadDB(io_context);
            co_spawn(io_context, listener.Run(), detached);//��ʼ�����˿�
            co_spawn(io_context, objectMap::expiredKeyCollecting, detached);//������������
            RDBSaver::saveDB(io_context);
            
            io_context.run();
        }
        catch (exception& e)
        {
            fmt::print("Exception: {}\n", e.what());
        }
    }
}
int main()
{

#if defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //MSVC�ڴ�й©��⣬��ctrl+c�رճ���(������ֱ�ӹرմ���)��������ڴ�й©����������ڻ�����ʾ
#endif
    myredis::run();
    return 0;
}