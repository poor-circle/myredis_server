#include "stdafx.h"
#include "RDBSaver.h"
#include "object.hpp"
#include "ObjectVisitor/serialize.h"
#include "ObjectVisitor/unserialize.h"
#include "threadPool.h"
#include "AOFSaver.h"
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

    asio::awaitable<void> RDBSaver::saveDBDetail(asio::io_context& ioc)
    {
        string tmp;
     
        AOFSaver::saveToTempFile();
        //TODO:fork on linux
        tmp=visitor::MultiDBSerialize();//������ݿ����л�

        bool waiting =true;//�ж��Ƿ�Ҫ�ȴ�
        asio::steady_timer clk(ioc);
        asio::post(getThreadPool(), [&] //��д���̵Ĺ��������̳߳�������һ���߳�ִ��
        {
            FILE* fp = nullptr;
            this_thread::sleep_for(30s);
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
            //ioc.post����������񽻸�ioc���ڵ����߳�ִ�У���֤���̰߳�ȫ
        });
        if (waiting)
        {
            clk.expires_after(steady_clock::duration::max());//�ȵ���������
            try
            {
                co_await clk.async_wait(asio::use_awaitable);
                //һֱ�ȵ��ļ�io��ɣ�����һ���߳�������ϼ�ʱ��
            }
            catch (const exception& e){}
        }
        AOFSaver::moveTempFile();
        co_return;
    }

    void RDBSaver::saveDB(asio::io_context &ioc)
    {
        asio::co_spawn(ioc, [&ioc] ()->asio::awaitable<void>
        {
            asio::steady_timer clk(ioc);
            clk.expires_after(1s);//���һ��ʱ�䱣�����
            while (true)
            {
                co_await clk.async_wait(asio::use_awaitable);//˯��,�ȴ�����
                assert((fmt::print("start RDBSave\n"), 1));
                co_await saveDBDetail(ioc);//�������ݿ�
                assert((fmt::print("end RDBSave\n"), 1));
                clk.expires_after(getDurationTime());
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

