
#include "stdafx.h"
#include "Listener.h"
#include "benchmark.h"
#include "ObjectVisitor/serialize.h"
#include "RDBSaver.h"
#include "AOFSaver.h"
#include "threadPool.h"
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


namespace myredis
{
    #include "namespace.i"
    static void run()
    {
        //Test();//debug时执行单元测试
        try
        {
            asio::io_context io_context;
            asio::signal_set signals(io_context,SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto) 
            { 
                io_context.stop();
            });
            myredis::Listener listener(io_context, myredis::defaultPort);
            RDBSaver::loadDB();//加载RDB文件
            AOFSaver::aofload();//加载AOF文件
            co_spawn(io_context, listener.Run(), detached);//开始监听端口
            co_spawn(io_context, objectMap::expiredKeyCollecting, detached);//运行垃圾回收
            RDBSaver::saveDB(io_context);//定时保存RDB文件
            io_context.run();
            getThreadPool().join();
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
    //MSVC内存泄漏侦测，按ctrl+c关闭程序(而不是直接关闭窗口)，如果有内存泄漏，则输出窗口会做提示
#endif
    myredis::run();
    return 0;
}