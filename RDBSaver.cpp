#include "stdafx.h"
#include "RDBSaver.h"
#include "object.hpp"
#include "ObjectVisitor/serialize.h"
#include "ObjectVisitor/unserialize.h"
namespace myredis
{
#include"namespace.i"


    std::chrono::seconds& RDBSaver::getDurationTime()
    {
        static seconds sec=60s;
        return sec;
    }

    void RDBSaver::setDurationTime(std::chrono::seconds time_duration)
    {
        getDurationTime() = time_duration;
    }

    asio::awaitable<void> RDBSaver::saveDBDetail(asio::steady_timer& clk, asio::io_context& ioc)
    {
        string tmp;
        tmp=visitor::MultiDBSerialize();
        std::atomic<bool> waiting =true;
        std::thread work([&] ()
        {
            FILE* fp=nullptr;
            try
            {
                fp = fopen("backup2.mrdb", "wb");
                if (fp == nullptr)
                    throw exception("RDBSaver failed when open file");
                if (fwrite(tmp.data(), sizeof(char), tmp.size(), fp) != tmp.size())
                    throw exception("RDBSaver failed when writing RDB file to disk");
            }
            catch (const exception& e)
            {
                if (fp!=NULL)
                    fclose(fp);
                printlog(e);
                waiting = false;
                return;
            };
            fclose(fp);
            try
            {
                remove("backup.mrdb");//allow failed
                if (rename("backup2.mrdb", "backup.mrdb"))
                    throw exception("RDBSaver failed when rename RDB file");
            }
            catch (const exception& e)
            {
                printlog(e);
            };
            waiting = false;
        });
        work.detach();
        while (waiting)
        {
            clk.expires_after(1s);
            co_await clk.async_wait(asio::use_awaitable);
        }
        co_return;
    }

    void RDBSaver::saveDB(asio::io_context &ioc)
    {
        asio::co_spawn(ioc, [&ioc] ()->asio::awaitable<void>
        {
            asio::steady_timer clk(ioc);
            while (true)
            {
                clk.expires_after(getDurationTime());//间隔一段时间保存磁盘
                co_await clk.async_wait(asio::use_awaitable);//睡觉,等待唤醒
                assert((fmt::print("start RDBSave\n"), 1));
                co_await saveDBDetail(clk,ioc);//保存数据库
                assert((fmt::print("end RDBSave\n"), 1));
            }
        }, detached);
    }
    void RDBSaver::loadDB(asio::io_context& ioc)
    {
        FILE* fp = nullptr;
        fp = fopen("backup.mrdb", "rb");
        if (fp == nullptr) return;
        visitor::multiDBUnserialize(fp);
        fclose(fp);
    }
}

