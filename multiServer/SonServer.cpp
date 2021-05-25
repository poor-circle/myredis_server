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
        ioc(ioc), listen_port(port),clk(ioc), timeout_clk(ioc){}

    void SonServer::setCas(serverCas cas)
    {
        this->cas = cas;
    }

    SonServer::serverCas SonServer::getCas()
    {
        return cas;
    }

    SonServer& SonServer::getSonServer()
    {
        return *_getSonServer().get();
    }

    void SonServer::rawSonServer(asio::io_context& ioc, uint16_t port)
    {
        _getSonServer() = std::make_unique<SonServer>(ioc, port);
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
                *soc = co_await acceptor.async_accept(asio::use_awaitable);
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
                asio::steady_timer clk(ioc);
                clk.expires_after(asio::steady_timer::duration::max());
                uint64_t sz = data.size();

                //�Զ����Ƹ�ʽ�������ݿ�����
                co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                co_await soc->async_send(asio::buffer(data.data(), data.size()), asio::use_awaitable);
                //�Զ����Ƹ�ʽ���ͻ�����ָ��
                while (!que.empty())
                {
                    sz = que.front().size();
                    if (sz != 0)
                    {
                        co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                        for (auto& e : que.front())
                        {
                            sz = e.size();
                            co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                            co_await soc->async_send(asio::buffer(e.data(), e.size()), asio::use_awaitable);
                        }
                    }
                    que.pop();
                }
                //֪ͨ�ӷ�����ͬ�����
                setCas(serverCas::sync);
                sz = 0;
                co_await soc->async_send(asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                //ͬ����ɣ��ȴ��쳣�¼�����������쳣�����½������ӣ�
                clk.expires_after(asio::steady_timer::duration::max());
                co_await clk.async_wait(asio::use_awaitable);
            }
            catch (const std::exception& e)
            {
                printlog(e);
            }
        }
	}

    asio::awaitable<void> SonServer::send(std::vector<string>& cmd,seconds cmd_used_time)
    {
        static char buf[1];
        static auto timeout_controll = [this]()->asio::awaitable<void>
        {
            try
            {
                co_await timeout_clk.async_wait(asio::use_awaitable);
                clk.cancel();//�ر����ӷ�����������
            }
            catch (const std::exception& e){}
            co_return;
        };
        if (getCas() == serverCas::sync)
        {
            try
            {
                co_await soc->async_send(asio::buffer(cmd.data(), cmd.size()), asio::use_awaitable);//��������
                timeout_clk.expires_after(seconds(son_server_expire_seconds) + son_server_speed_lower_rate * cmd_used_time);//���ó�ʱʱ��
                asio::co_spawn(ioc, timeout_controll, asio::detached);
                co_await soc->async_receive(asio::buffer(buf), asio::use_awaitable);//�ظ�һ���ֽڣ���ʾ����ִ�����
                timeout_clk.cancel();
            }
            catch (const std::exception& e)
            {
                clk.cancel();//�ر����ӷ�����������
                timeout_clk.cancel();
            }
        }
        else if (getCas()==serverCas::rawing)
        {
            que.push(cmd);
        }
        co_return;
    }
}

