#include "stdafx.h"
#include "AOFSaver.h"


namespace myredis
{
#include"namespace.i"
    // ��command���벢׷�ӵ�dest��
    void AOFSaver::aofencode(vector<string>& command,string& dest)  //�����ظ�
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
    // ��fp�ж�ȡһ�����command(��vector����ʽ)
    void AOFSaver::aofdecode(FILE* fp, vector<string>& command) {
        command.clear();
        // ��ȡ*���������
        array<char, 1024> buf;
        try {
            fgets(buf.data(), 1024, fp);
            auto iter = find(buf.begin(), buf.end(), '*');
            if (iter == buf.end()) {
                throw std::exception("");
            }
            auto commandLine = lexical_cast<size_t>(*(++iter));

            for(size_t i=0;i<commandLine;i++){
                // �õ����� $x\r\n
                buf.fill('\0');
                fgets(buf.data(),1024,fp);
                iter = find(buf.begin(), buf.end(), '$');
                if (iter == buf.end()) {
                    throw std::exception("");
                }
                auto strCnt = lexical_cast<size_t>(*(++iter));
                buf.fill('\0');
                // �õ�ָ��
                fgets(buf.data(), 1024, fp);
                string tmp(buf.data());
                // ��ȡ[0,size)���Ӵ�����
                command.emplace_back(tmp.substr(0,strCnt));
            }
            return ;
        }
        catch (const std::exception& e)
        {
            if (*e.what() == '\0')
                printlogByStr("���ݿ����ݽ���ʧ�ܣ�");
            else
                printlog(e);
        }
  
    }
    /* command�����Ѿ��������д����,fp�Ǵ�����ļ�ָ�� */
    void AOFSaver::aofwriter(vector<string>&& command, FILE* fp)
    {
        // �ж�command�Ƿ���д����
        string funcStr = command[0];
        // Q:������ʵ�е��ظ�����ִ��ָ����ǿ�Ҳ�����������Ĳ������ܷ���ִ��ָ���������writer�أ�
        boost::algorithm::to_lower(funcStr);
        auto funcTable = getfuncManager();
        auto iter = funcTable.find(funcStr);
        if (iter != funcTable.end()&&iter->second.type== func::funcType::write) {
            try
            {
                if (fp == nullptr)
                    throw exception("AOF failed when open file");
                // ��������б���
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
        // ����
        // ����ʲôʱ����һ��ָ��
        vector<string> command;

        while (feof(fp)) {
            // һ�ζ�ȡһ������ 
            aofdecode(fp,command);
            // ִ������
            string reply;

            // Q:������Ҫ��Э����
            funcControll(func::context(std::move(command), *self.get()), reply);
            // Ӧ�ò���Ҫ������
            //co_await asio::async_write(self->socket, asio::buffer(reply.data(), reply.size()), use_awaitable);
        }
    }
}

