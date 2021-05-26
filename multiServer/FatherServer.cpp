#include "stdafx.h"
#include "fatherServer.h"
#include "../RDBSaver.h"
#include "../funcController.h"
#include "../Func/funcManager.h"
namespace myredis
{
    using namespace std::chrono;

    asio::awaitable<std::vector<string>> getCmd(asio::ip::tcp::socket& soc)
    {
        size_t sz;
        co_await asio::async_read(soc, asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
        std::vector<string> cmd;
        cmd.reserve(sz);
        for (size_t i = 0; i < sz; ++i)
        {
            size_t sz;
            cmd.emplace_back();
            co_await asio::async_read(soc, asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
            cmd.back().resize(sz);
            co_await asio::async_read(soc, asio::buffer(cmd.back().data(), sz), asio::use_awaitable);
        }
        co_return cmd;
    }

    asio::awaitable<void> FatherServer::sync(std::string_view address,uint16_t port)
    {
        while (getCas() != serverCas::sync)
        {
            try
            {
                soc.close();
                cas = serverCas::notConnect;
                asio::ip::tcp::resolver res(ioc);
                auto ep = asio::ip::tcp::endpoint(asio::ip::make_address_v4(address), port);
                auto g = co_await res.async_resolve(ep, asio::use_awaitable);
                std::vector v = { ep };
                co_await asio::async_connect(soc, v.begin(),v.end(), asio::use_awaitable);
                //同步完成
                cas = serverCas::sync;
                co_await asio::async_read(soc, asio::buffer(&faServerNormalPort, sizeof(faServerNormalPort)), asio::use_awaitable);
                auto port = getDefaultPort();
                co_await soc.async_send(asio::buffer(&port, sizeof(port)), asio::use_awaitable);
                uint64_t sz;
                co_await asio::async_read(soc, asio::buffer(&sz, sizeof(sz)), asio::use_awaitable);
                string buf;
                buf.resize(sz);
                co_await asio::async_read(soc, asio::buffer(buf.data(), sz), asio::use_awaitable);
                co_await RDBSaver::saveDBToDesk(ioc, buf);
                RDBSaver::loadDB();
                auto session = BaseSession::create(ioc, asio::ip::tcp::socket(ioc));
                session->setIsInnerSession(true);
                //同步一阶段
                char ch;
                for (; co_await asio::async_read(soc, asio::buffer(&ch, sizeof(ch)), asio::use_awaitable), ch != 1;)
                {
                    std::vector<string> cmd;
                    while (true)
                    {
                        cmd = co_await getCmd(soc);
                        if (cmd.size() == 0) break;
                        func::funcRunner(getfuncManager().find(cmd[0])->second, func::context(std::move(cmd), *session));
                    }
                    co_await asio::async_write(soc, asio::buffer(&ch, sizeof(ch)), asio::use_awaitable);
                }
                //同步二阶段
                std::vector<string> cmd;
                while (true)
                {
                    cmd = co_await getCmd(soc);
                    if (cmd.size() == 0) break;
                    func::funcRunner(getfuncManager().find(cmd[0])->second, func::context(std::move(cmd), *session));
                    co_await asio::async_write(soc, asio::buffer(&ch, sizeof(ch)), asio::use_awaitable);
                }
                //同步完成
                cas = serverCas::sync;
            }
            catch (const std::exception& e)
            {
                printlog(e);
            }
        }
    }
    asio::awaitable<void> FatherServer::run()
    {
        std::vector<string> cmd;

        try
        {

            auto session = BaseSession::create(ioc, asio::ip::tcp::socket(ioc));
            session->setIsInnerSession(true);
            while (cas==serverCas::sync)
            {
                cmd = co_await getCmd(soc);
                auto info = getfuncManager().find(cmd[0])->second;
                auto ret = co_await func::funcRunner(info, func::context(std::move(cmd), *session));
                if (ret.has_value())
                    co_await asio::async_write(soc, asio::buffer(ret.value().data(), ret.value().size()), asio::use_awaitable);
                else
                {
                    throw std::exception("error fatherserver command!");
                }
            }
        }
        catch (std::exception& e)
        {
            cas = serverCas::no_father;
        }
    }
    FatherServer::serverCas FatherServer::getCas()
    {
        return cas;
    }
    void FatherServer::setFatherServer(asio::io_context& ioc)
    {
        _getFatherServer() = std::make_unique<FatherServer>(ioc);
    }
    FatherServer* FatherServer::getFatherServer()
    {
        return _getFatherServer().get();
    }
    asio::ip::tcp::endpoint FatherServer::getAddress()
    {
        return { soc.remote_endpoint().address(),faServerNormalPort };
    }
    std::unique_ptr<FatherServer>& FatherServer::_getFatherServer()
    {
        static std::unique_ptr<FatherServer> server;
        return server;
    }
    FatherServer::FatherServer(asio::io_context& ioc):ioc(ioc),soc(ioc),cas(serverCas::no_father){}
}

