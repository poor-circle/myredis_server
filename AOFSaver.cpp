#include "stdafx.h"
#include "AOFSaver.h"
#include <filesystem>

namespace myredis
{
#include"namespace.i"
    // 将command编码并追加到dest上
    void AOFSaver::aofencode(vector<string>& command,FILE* dest)  //批量回复
    {
        fmt::print(dest,"*{}\r\n",command.size());
        for (auto &str : command) {
            fmt::print(dest,"${}\r\n{}\r\n",str.size(),str);
        }
        return ;
    }
    // 从fp中读取一条命令到command(以vector的形式)


    vector<string> AOFSaver::aofdecode(FILE* fp) 
    {
        char buf[4];
        vector<string> command;
        static std::exception e("parse error");
        if (fread(buf, sizeof(char), 1, fp) == 1)
        {
            if (buf[0] != '*') 
                throw e;
            size_t sz;
            if (fscanf(fp,"%zu",&sz) != 1)
                throw e;
            if (fread(buf, sizeof(char), 2, fp) != 2)
                throw e;
            if (buf[0]!='\r'||buf[1]!='\n') 
                throw e;
            command.resize(sz);
            for (size_t i = 0; i < sz; ++i)
            {
                size_t strsz;
                if (fread(buf, sizeof(char), 1, fp) != 1) 
                    throw e;
                if (buf[0] != '$') 
                    throw e;
                if (fscanf(fp, "%zu", &strsz) != 1) 
                    throw e;
                command[i].resize(strsz);
                if (fread(buf, sizeof(char), 2, fp) != 2) 
                    throw e;
                if (buf[0] != '\r' || buf[1] != '\n') 
                    throw e;
                if (fread(command[i].data(), sizeof(char), strsz, fp) != strsz)
                    throw e;
                if (fread(buf, sizeof(char), 2, fp) != 2) 
                    throw e;
                if (buf[0] != '\r' || buf[1] != '\n') 
                    throw e;
            }
        }
        return command;
    }
    FILE*& AOFSaver::getFile(size_t dBIndex)
    {
        static bool isRaw = true;
        static vector<FILE*> fp = vector<FILE*>(data_base_count);
        static std::mutex lock;
        if (isRaw)
        {
            std::lock_guard<std::mutex> lk(lock);
            filesystem::create_directory("aof");
            if (isRaw)
            {
                for (size_t i = 0; i < data_base_count; ++i)
                {
                    fp[i] = fopen(("aof/" + to_string(i) + ".maof").c_str(), "wb");
                }
                isRaw = false;
            }
        }
        return fp.at(dBIndex);
    }
    void AOFSaver::saveToTempFile()
    {
        for (size_t i = 0; i < data_base_count; ++i)
        {
            fclose(AOFSaver::getFile(i));
            auto name = ("aof/" + to_string(i) + ".maof2");
            if ((AOFSaver::getFile(i) = fopen(name.c_str(), "wb"))==nullptr)
                throw exception("AOFSaver failed when create new AOF file");
        }
    }

    void AOFSaver::moveTempFile()
    {
        for (size_t i = 0; i < data_base_count; ++i)
        {
            fclose(AOFSaver::getFile(i));
            auto name = ("aof/" + to_string(i) + ".maof"),name2=name+"2";
            remove(name.c_str());
            if (rename(name2.c_str(), name.c_str()))
                throw exception("AOFSaver failed when rename AOF file");
            AOFSaver::getFile(i)=fopen(name.c_str(),"wb");
        }
    }
    /* command代表已经处理过的写命令,fp是存入的文件指针 */
    void AOFSaver::aofwriter(vector<string>& command, FILE* fp)
    {
        try
        {
            if (fp == nullptr)
                throw exception("AOF failed when open file");
            aofencode(command,fp);
        }
        catch (const exception& e)
        {
            printlog(e);
            return;
        };
    }

    void AOFSaver::aofload() noexcept
    {
        // 解码
        // 读到什么时候算一条指令
        for (size_t i = 0; i < data_base_count; ++i)
        {
            auto s = "aof/" + to_string(i) + ".maof";
            FILE* fp = fopen(s.c_str(), "rb");
            if (fp == nullptr) return;
            try
            {
                asio::io_context ioc;
                //创建虚假的客户端连接上下文
                auto session = BaseSession::create(ioc, asio::ip::tcp::socket(ioc));
                session->setDataBaseID(i);
                while (!feof(fp))
                {
                    // 一次读取一个命令 
                    auto command = aofdecode(fp);
                    if (command.size() == 0) break;
                    auto iter = getfuncManager().find(command[0]);
                    if (iter == getfuncManager().end())
                        throw exception("no such aof command!");
                    iter->second.syncptr(func::context(std::move(command), *session));
                    //we don't need return value
                }
            }
            catch (const exception& e)
            {
                printlog(e);
            }
            if (fp != nullptr)
                fclose(fp);
        }
    }
}

