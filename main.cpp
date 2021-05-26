
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
        //Test();//debug时执行单元测试
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
                    ioc.run();//尝试与父服务器同步
                    ioc.reset();
                    co_spawn(ioc, FatherServer::getFatherServer()->run(), detached);//与父服务器的写同步连接
                }
            }
            //启动主服务器
            asio::signal_set signals(ioc, SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto)
                {
                    ioc.stop();
                });
            myredis::Listener listener(ioc, main_port);
            if (FatherServer::getFatherServer()->getCas() != FatherServer::serverCas::sync)
            {
                RDBSaver::loadDB();//加载RDB文件
                AOFSaver::aofload();//加载AOF文件
            }
            co_spawn(ioc, listener.Run(), detached);//开始监听端口
            co_spawn(ioc, objectMap::expiredKeyCollecting, detached);//运行垃圾回收
            if (enable_backup_server)
            {
                SonServer::rawSonServer(ioc, inner_port);
                co_spawn(ioc, SonServer::getSonServer()->run(), detached);//开始监听并准备与子服务器同步
            }
            RDBSaver::saveDB(ioc);//定时保存RDB文件


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
    //MSVC内存泄漏侦测，按ctrl+c关闭程序(而不是直接关闭窗口)，如果有内存泄漏，则输出窗口会做提示
#endif
    myredis::mainRun(argc,argv);
    
    return 0;
}