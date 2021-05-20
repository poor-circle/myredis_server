
#include "stdafx.h"
#include "Listener.h"
#include "benchmark.h"
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "ObjectVisitor/serialize.h"
#endif
namespace myredis
{
    #include "namespace.i"
    static void run()
    {
        Test();//debug时执行单元测试
        try
        {
            
            asio::io_context io_context(1);
            asio::signal_set signals(io_context,SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto) 
            { 
                io_context.stop();
            });
            myredis::Listener listener(io_context, myredis::defaultPort);
            
            co_spawn(io_context, listener.Run(), detached);//开始监听端口
            co_spawn(io_context, objectMap::expiredKeyCollecting, detached);//运行垃圾回收
            
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
    //MSVC内存泄漏侦测，按ctrl+c关闭程序(而不是直接关闭窗口)，如果有内存泄漏，则输出窗口会做提示
#endif
    //myredis::run();
    // test 序列化
    myredis::object temp= std::make_unique<myredis::deque<myredis::string>>();
    for (int i = 0; i < 66; i++) {
        std::get<std::unique_ptr<myredis::deque<myredis::string>>>(temp)->push_back(myredis::string()+ (char)i);
    }


    auto str = myredis::visitor::serialize<std::unique_ptr<myredis::deque<myredis::string>>>(temp);
    for (auto i : str) {
        std::cout << (unsigned int)i << std::endl;
    }
    return 0;
}