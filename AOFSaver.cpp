#include "stdafx.h"
#include "AOFSaver.h"


namespace myredis
{
#include"namespace.i"
    // 将command编码并追加到dest上
    void AOFSaver::aofencode(vector<string>& command,string& dest)  //批量回复
    {

        fmt::format_to(
            back_inserter(dest),
            FMT_COMPILE("*{}\r\n"),
            command.size()
        );
        for (auto str : command) {
            fmt::format_to
            (
                back_inserter(dest),
                FMT_COMPILE("${}\r\n{}\r\n"),
                str.size(),
                str
            );
        }
        return ;
    }
    // 从fp中读取一条命令到command(以vector的形式)
    void AOFSaver::aofdecode(FILE* fp, vector<string>& command) {
        command.clear();
        // 读取*后面的整数
        array<char, 1024> buf;
        try {
            fgets(buf.data(), 1024, fp);
            auto iter = find(buf.begin(), buf.end(), '*');
            if (iter == buf.end()) {
                throw std::exception("");
            }
            auto commandLine = lexical_cast<size_t>(*(++iter));

            for(size_t i=0;i<commandLine;i++){
                // 得到长度 $x\r\n
                buf.fill('\0');
                fgets(buf.data(),1024,fp);
                iter = find(buf.begin(), buf.end(), '$');
                if (iter == buf.end()) {
                    throw std::exception("");
                }
                auto strCnt = lexical_cast<size_t>(*(++iter));
                buf.fill('\0');
                // 得到指令
                fgets(buf.data(), 1024, fp);
                string tmp(buf.data());
                // 截取[0,size)的子串即可
                command.emplace_back(tmp.substr(0,strCnt));
            }
            return ;
        }
        catch (const std::exception& e)
        {
            if (*e.what() == '\0')
                printlogByStr("数据库内容解析失败！");
            else
                printlog(e);
        }
  
    }
    /* command代表已经处理过的写命令,fp是存入的文件指针 */
    void AOFSaver::aofwriter(vector<string>&& command, FILE* fp)
    {
        // 判断command是否是写命令
        string funcStr = command[0];
        // Q:这里其实有点重复，在执行指令的那块也进行了这样的操作，能否在执行指令后调用这个writer呢？
        boost::algorithm::to_lower(funcStr);
        auto funcTable = getfuncManager();
        auto iter = funcTable.find(funcStr);
        if (iter != funcTable.end()&&iter->second.type== func::funcType::write) {
            try
            {
                if (fp == nullptr)
                    throw exception("AOF failed when open file");
                // 对命令进行编码
                string tmp;
                aofencode(command,tmp);
                if (fwrite(tmp.data(), sizeof(char), tmp.size() + 1, fp) != tmp.size() + 1)
                    throw exception("AOF failed when writing");
            }
            catch (const exception& e)
            {
                if (fp != NULL)
                    fclose(fp);
                printlog(e);
                return;
            };
        }
    }

    void AOFSaver::aofload(FILE* fp, unique_ptr<BaseSession> self)
    {
        // 解码
        // 读到什么时候算一条指令
        vector<string> command;

        while (feof(fp)) {
            // 一次读取一个命令 
            aofdecode(fp,command);
            // 执行命令
            string reply;

            // Q:这里需要用协程吗？
            funcControll(func::context(std::move(command), *self.get()), reply);
            // 应该不需要发送了
            //co_await asio::async_write(self->socket, asio::buffer(reply.data(), reply.size()), use_awaitable);
        }
    }
}

