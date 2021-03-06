#include "stdafx.h"
#include "RDBSaver.h"
#include "object.hpp"
#include "ObjectVisitor/serialize.h"
#include "ObjectVisitor/unserialize.h"
#include "threadPool.h"
#include "AOFSaver.h"
#include "multiServer/FatherServer.h"
namespace myredis
{
#include"namespace.i"


    std::chrono::seconds& RDBSaver::getDurationTime()
    {
        static seconds sec=seconds(RDB_Duration_second);
        return sec;
    }

    void RDBSaver::setDurationTime(std::chrono::seconds time_duration)
    {
        getDurationTime() = time_duration;
    }

    asio::awaitable<void> RDBSaver::saveDBToDesk(asio::io_context& ioc,string&data)
    {
        bool waiting = true;//判断是否要等待
        asio::steady_timer clk(ioc);
        asio::post(getThreadPool(), [&] //将写磁盘的工作交给线程池中另外一个线程执行
            {
                FILE* fp = nullptr;
                try
                {
                    fp = fopen("backup2.mrdb", "wb");
                    if (fp == nullptr)
                        throw exception("RDBSaver failed when open file");
                    if (fwrite(data.data(), sizeof(char), data.size(), fp) != data.size())
                        throw exception("RDBSaver failed when writing RDB file to disk");
                }
                catch (const exception& e)
                {
                    if (fp != NULL)
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
                ioc.post([&] {waiting = false; clk.cancel(); });
                //ioc.post：将这个任务交给ioc所在的主线程执行，保证了线程安全
            });
        if (waiting)
        {
            clk.expires_after(steady_clock::duration::max());//等到猴年马月
            try
            {
                co_await clk.async_wait(asio::use_awaitable);
                //一直等到文件io完成，另外一个线程主动打断计时器
            }
            catch (const exception& e) {}
        }
        co_return;
    }

    asio::awaitable<void> RDBSaver::saveDBDetail(asio::io_context& ioc)
    {
        string tmp;
        auto father = FatherServer::getFatherServer();
        if (father!=nullptr&&father->getCas() != FatherServer::serverCas::sync)
        {
            AOFSaver::saveToTempFile();
            //TODO:fork on linux
            tmp=visitor::MultiDBSerialize();//完成数据库序列化

            co_await saveDBToDesk(ioc, tmp);
        
            AOFSaver::moveTempFile();
        }
        
        co_return;
    }
    std::unique_ptr<asio::steady_timer>& getInnerClk()
    {
        static std::unique_ptr<asio::steady_timer> clk;
        return clk;
    }
    void RDBSaver::saveNow()
    {
        getInnerClk()->cancel();
    }
    void RDBSaver::saveDB(asio::io_context &ioc)
    {
        getInnerClk() = std::make_unique<asio::steady_timer>(ioc);
        asio::co_spawn(ioc, [&ioc] ()->asio::awaitable<void>
        {
            
            asio::steady_timer& clk=*getInnerClk();
            while (true)
            {
                clk.expires_after(getDurationTime());
                try
                {
                    co_await clk.async_wait(asio::use_awaitable);//睡觉,等待唤醒
                }
                catch (const std::exception&)
                {

                }
                assert((fmt::print("start RDBSave\n"), 1));
                co_await saveDBDetail(ioc);//保存数据库
                assert((fmt::print("end RDBSave\n"), 1));
            }
        }, detached);
    }
    void RDBSaver::loadDB()
    {
        FILE* fp = nullptr;
        fp = fopen("backup.mrdb", "rb");
        if (fp == nullptr) return;
        visitor::multiDBUnserialize(fp);
        fclose(fp);
    }
}

