#include "stdafx.h"
#include "SonServer.h"
#include "../code.h"
#include "../ObjectVisitor/serialize.h"
#include "../threadPool.h"
namespace myredis
{
    using namespace std::chrono;

    std::unique_ptr<SonServer>& SonServer::_getSonServer()
    {
        static std::unique_ptr<SonServer> server;
        return server;
    }
    SonServer::SonServer(asio::io_context& ioc, uint16_t port):
        ioc(ioc), listen_port(port),clk(ioc),sendClk(ioc),
        cas(serverCas::notConnect){}

    asio::ip::tcp::endpoint SonServer::getAddress()
    {
        return { soc->remote_endpoint().address(),sonServerNormalPort };
    }

    void SonServer::setCas(serverCas cas)
    {
        this->cas = cas;
    }

    SonServer::serverCas SonServer::getCas()
    {
        return cas;
    }

    SonServer* SonServer::getSonServer()
    {
        return _getSonServer().get();
    }

    void SonServer::rawSonServer(asio::io_context& ioc, uint16_t port)
    {
        _getSonServer() = std::make_unique<SonServer>(ioc, port);
    }

    asio::awaitable<void> sendCmdInner(const std::vector<string>& cmd, asio::ip::tcp::socket& soc)
    {
        auto sz = cmd.size();
        co_await soc.async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
        for (auto& e : cmd)
        {
            sz = e.size();
            co_await soc.async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
            co_await soc.async_send(asio::buffer(e.data(), e.size()), asio::use_awaitable);
        }
        co_return;
    }

    asio::awaitable<void> SonServer::sendCmd(std::vector<string>&& cmd)
    {
        if (cmdQueue.empty())
        {
            sendClk.cancel();
        }
        cmdQueue.push_back(cmd);
        co_return;
    }

	asio::awaitable<void> SonServer::run()
	{
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::acceptor acceptor(executor, { asio::ip::tcp::v4(), listen_port });
        fmt::print
        (
            "*{0:*^{2}}*\n"
            "*{0: ^{2}}*\n"
            "*{1: ^{2}}*\n"
            "*{0: ^{2}}*\n"
            "*{0:*^{2}}*\n\n",
            "", "myredis start listening subserver at port:" + std::to_string(listen_port), 75
        );
        while (true)
        {
            try
            {
                setCas(serverCas::notConnect);
                soc = std::make_unique<asio::ip::tcp::socket>(co_await acceptor.async_accept(asio::use_awaitable));
                assert((fmt::print
                (
                    "|{0:-^{2}}|\n"
                    "|{1: ^{2}}|\n"
                    "|{0:-^{2}}|\n\n",
                    "",
                    fmt::format("connect from subserver {}:{}", soc->remote_endpoint().address(), soc->remote_endpoint().port()),
                    75
                ), 1));
                auto data = visitor::MultiDBSerialize();
                setCas(serverCas::rawing);
                uint64_t sz = data.size();

                //发送自己的客户端端口
                uint16_t port = getDefaultPort();
                co_await soc->async_send(asio::buffer(&port, sizeof(port)), asio::use_awaitable);

                //接收对方的客户端端口
                co_await asio::async_read(*soc, asio::buffer(&sonServerNormalPort, sizeof(uint16_t)), asio::use_awaitable);

                //以二进制格式发送数据库内容
                co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                co_await soc->async_send(asio::buffer(data.data(), data.size()), asio::use_awaitable);
                //以二进制格式发送缓冲区指令

                //开始同步
                char ch = 0;
                while (cmdQueue.size() >= max_un_sync_limit)
                {
                    //通知子服务器本次同步开始
                    co_await soc->async_send(asio::buffer(&ch, sizeof(ch)), asio::use_awaitable);
                    while (!cmdQueue.empty())
                    {
                        if (cmdQueue.front().size() != 0)
                        {
                            co_await sendCmdInner(cmdQueue.front(),*soc);
                        }
                        cmdQueue.pop_front();
                    }
                    //通知子服务器本次同步完成
                    sz = 0;
                    co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                    //等待回复
                    co_await asio::async_read(*soc, asio::buffer(&ch, sizeof(ch)), asio::use_awaitable);
                }
                //通知子服务器进入同步第二阶段
                ch = 1;
                co_await soc->async_send(asio::buffer(&ch, sizeof(ch)), asio::use_awaitable);
                while (!cmdQueue.empty())
                {
                    if (cmdQueue.front().size() != 0)
                    {
                        co_await sendCmdInner(cmdQueue.front(), *soc);
                        //等待回复
                        co_await asio::async_read(*soc, asio::buffer(&ch, sizeof(ch)), asio::use_awaitable);
                    }
                    cmdQueue.pop_front();
                }
                //通知子服务器同步完成
                setCas(serverCas::sync);
                sz = 0;
                co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                //通知子服务器同步结束
                
                //启动连接器
                asio::co_spawn(ioc, [this]()->asio::awaitable<void>
                {
                    while (cas != serverCas::notConnect)
                    {
                        try
                        {
                            while (cmdQueue.size())
                            {
                                auto& cmd = cmdQueue.front();
                                co_await sendCmdInner(cmd, *soc);
                                cmdQueue.pop_front();
                            }
                            sendClk.expires_after(asio::steady_timer::duration::max());
                            try
                            {
                                sendClk.async_wait(asio::use_awaitable);
                            }
                            catch (const std::exception&) {}
                        }
                        catch (const std::exception&)
                        {
                            closeConnect();
                        }
                    }
                }, asio::detached);

                //启动接收器
                asio::co_spawn(ioc, [this]()->asio::awaitable<void>
                {
                    char ch[1];
                    while (cas != serverCas::notConnect)
                    {
                        try
                        {
                            co_await asio::async_read(*soc, asio::buffer(&ch, sizeof(char)), asio::use_awaitable);
                            if (clkQueue.size())
                            {
                                clkQueue.front()->cancel();
                                clkQueue.pop_front();
                            }
                        }
                        catch (const std::exception& e)
                        {
                            closeConnect();
                        }
                    }
                }, asio::detached);


                //定时检测连接是否断开
                asio::co_spawn(ioc, [this]()->asio::awaitable<void>
                {
                    auto cmd = std::vector<string>{ "ping" };
                    asio::steady_timer clk(ioc);
                    while (true)
                    {
                        clk.expires_after(seconds(son_server_expire_seconds));
                        clk.async_wait(asio::use_awaitable);
                        if (cmdQueue.empty())
                            co_await send(std::vector<string>{cmd}, 0s);//发送命令
                    }
                }, asio::detached);
                //同步完成，等待异常事件（如果出现异常则重新建立连接）
                clk.expires_after(asio::steady_timer::duration::max());
                co_await clk.async_wait(asio::use_awaitable);
            }
            catch (const std::exception& e)
            {
                printlog(e);
            }
        }
	}


    void SonServer::closeConnect()
    {
        if (cas != serverCas::notConnect)
        {
            printlogByStr("time out with subserver");
            clk.cancel();//关闭与子服务器的连接
            cas = serverCas::notConnect;
            cmdQueue.clear();
            while (clkQueue.size())
            {
                clkQueue.front()->expires_after(0s);
                clkQueue.pop_front();
            }
            sendClk.cancel();
            soc->shutdown(asio::socket_base::shutdown_both);
            soc->close();
        }
    }

    asio::awaitable<void> SonServer::send(std::vector<string>&& cmd,seconds cmd_used_time)
    {
        if (getCas() == serverCas::sync)
        {
            asio::steady_timer timeout_clk(ioc);
            timeout_clk.expires_after(seconds(son_server_expire_seconds) + son_server_speed_lower_rate * cmd_used_time);//设置超时时间
            co_await sendCmd(std::move(cmd));//发送命令
            try
            {
                co_await timeout_clk.async_wait(asio::use_awaitable);
                clk.cancel();//关闭与子服务器的连接
            }
            catch (const std::exception& e){}
        }
        else if (getCas()==serverCas::rawing)
        {
            cmdQueue.push_back(std::move(cmd));
        }
        co_return;
    }
}

