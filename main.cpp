
#include "stdafx.h"
#include "Listener.h"
#include "benchmark.h"
#include "ObjectVisitor/serialize.h"
#include "RDBSaver.h"
#include "AOFSaver.h"
#include "threadPool.h"
#include "multiServer/SonServer.h"

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
            RDBSaver::loadDB();//����RDB�ļ�
            AOFSaver::aofload();//����AOF�ļ�
            co_spawn(io_context, listener.Run(), detached);//��ʼ�����˿�
            co_spawn(io_context, objectMap::expiredKeyCollecting, detached);//������������
            if (enable_backup_server)
            {
                SonServer::rawSonServer(io_context,myredis::defaultInnerPort);
                SonServer::getSonServer().run();//��ʼ������׼�����ӷ�����ͬ��
            }
            RDBSaver::saveDB(io_context);//��ʱ����RDB�ļ�
            io_context.run();
            getThreadPool().join();
        }
        catch (exception& e)
        {
            fmt::print("Exception: {}\n", e.what());
        }
    }
}
int main(int argc,char **argv)
{

#if defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //MSVC�ڴ�й©��⣬��ctrl+c�رճ���(������ֱ�ӹرմ���)��������ڴ�й©����������ڻ�����ʾ
#endif
    if (argc == 3)
    {
        std::string ip = argv[1],port=argv[2];

    }
    myredis::run();
    
    return 0;
}