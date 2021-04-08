
#include"stdafx.h"
#include"Listener.h"

namespace myredis
{
    #include "namespace.h"
    static void run()
    {
        try
        {
            asio::io_context io_context(1);
            asio::signal_set signals(io_context, SIGINT, SIGTERM);
            signals.async_wait([&](auto, auto) { io_context.stop(); });
            myredis::Listener listener(io_context, 8848);

            co_spawn(io_context, listener.Run(), detached);
            
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
    myredis::run();
    return 0;
}