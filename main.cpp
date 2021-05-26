
#include "stdafx.h"
#include "Listener.h"
#include "benchmark.h"
#include "ObjectVisitor/serialize.h"
#include "RDBSaver.h"
#include "AOFSaver.h"
#include "threadPool.h"
#include "multiServer/SonServer.h"
#include "multiServer/FatherServer.h"

#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


namespace myredis
{
    #include "namespace.i"

    static void mainRun(int argc, char** argv)
    {
        //Test();//debugʱִ�е�Ԫ����
        try
        {
            asio::io_context ioc;
            FatherServer::setFatherServer(ioc);
            uint16_t& main_port = myredis::getDefaultPort();
            uint16_t& inner_port = myredis::getDefaultInnerPort();
            if (argc == 5)
            {
                try_lexical_convert<uint16_t>(argv[1], main_port);
                try_lexical_convert<uint16_t>(argv[2], inner_port);
                std::string ip = argv[3], port = argv[4];
                uint16_t port_u16;
                auto res = try_lexical_convert<uint16_t>(port, port_u16);
                if (res)
                {
                    co_spawn(ioc, FatherServer::getFatherServer()->sync(ip, port_u16), detached);
                    ioc.run();//�����븸������ͬ��
                    ioc.reset();
                    co_spawn(ioc, FatherServer::getFatherServer()->run(), detached);//�븸��������дͬ������
                }
            }
            //������������
            asio::signal_set signals(ioc, SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto)
                {
                    ioc.stop();
                });
            myredis::Listener listener(ioc, main_port);
            if (FatherServer::getFatherServer()->getCas() != FatherServer::serverCas::sync)
            {
                RDBSaver::loadDB();//����RDB�ļ�
                AOFSaver::aofload();//����AOF�ļ�
            }
            co_spawn(ioc, listener.Run(), detached);//��ʼ�����˿�
            co_spawn(ioc, objectMap::expiredKeyCollecting, detached);//������������
            if (enable_backup_server)
            {
                SonServer::rawSonServer(ioc, inner_port);
                co_spawn(ioc, SonServer::getSonServer()->run(), detached);//��ʼ������׼�����ӷ�����ͬ��
            }
            RDBSaver::saveDB(ioc);//��ʱ����RDB�ļ�


            ioc.run();
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
    myredis::mainRun(argc,argv);
    
    return 0;
}