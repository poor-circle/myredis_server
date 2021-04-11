#include "stdafx.h"
#include "Session.h"
#include "object.hpp"
#include "Func/funcManager.h"
namespace myredis
{
#include"namespace.h"
    class Echo :Session
    {
    public:
        static uint32_t sessionTypeCode;
        Echo(asio::io_context& ioc, tcp::socket socket) :Session(ioc, std::move(socket)) {};
        //��������С�ݶ�Ϊ2000���ֽ�
        constexpr static int BUFSIZE = 2000;//��������С�ݶ�Ϊ2000���ֽ�
        //����myredis�������ӣ��ڴ�֮ǰ���ѽ����������ӣ�
        virtual awaitable<void> Run()
        {
            array<char, BUFSIZE> buf;
            auto iterBegin = buf.begin(), iterEnd= buf.begin();
            //iterBegin��iterEnd��һ��ָ��
            //iterBegin:Ӧ�ô����￪ʼ��
            //iterEnd:�������������

            array<string, 6> replys =//���ڲ��ԵĻظ�
            {
                "+OK\r\n",
                "-error:no such key\r\n",
                ":4213425\r\n",
                "$6\r\nfoobar\r\n",
                "*4\r\n$3\r\nfoo\r\n$3\r\nbar\r\n$5\r\nHello\r\n$5\r\nWorld\r\n"
            };

            auto reply = replys.cbegin();
            while (true)
            {
                //��ȡ*�ź������������һ���ж����У�
                auto lineCount = co_await ReadULLAfter(buf,iterBegin, iterEnd,'*');
                vector<string> args;
                args.reserve(lineCount);
                fmt::print("line count:{}\n", lineCount);
                for (size_t i = 0; i < lineCount; ++i)
                {
                    //��ȡ'$'�ź������������һ���ж೤��
                    auto length = co_await ReadULLAfter(buf, iterBegin,iterEnd, '$');
                    fmt::print("line length:{}\n", length);
                    //������ǰû�������
                    co_await SkipLine(buf, iterBegin, iterEnd);
                    //��ȡ�̶���С���ַ�������������б���
                    args.emplace_back(co_await ReadFixChar(buf, iterBegin, iterEnd, length));
                    fmt::print("line context:\n{}\n", args.back());
                }
                //�����ĵ�һ���ַ����Ǻ������������Ƿ����������
                auto iter = getfuncManager().find(args[0]);
                //���������ڣ����ش�����Ϣ����ǰΪ�˿ͻ��˵��Է��㣬����һ�����ڲ��ԵĻظ�
                if (iter == getfuncManager().end())
                {
                    co_await asio::async_write(socket, asio::buffer(reply->data(), reply->size()), use_awaitable);
                    fmt::print("test reply:{}\n", *reply);
                    ++reply;
                    if (reply == replys.cend()) reply = replys.cbegin();
                }
                //�������ڣ����иú���������������ظ��ͻ���
                else
                {
                    auto reply =iter->second(std::move(args));
                    //���reply��Ϊ��
                    if (reply.has_value())
                    {
                        co_await asio::async_write(socket, asio::buffer(reply.value().data(), reply.value().size()), use_awaitable);
                        fmt::print("reply:{}\n", reply.value());
                    }
                    //replyΪ�գ���������ʱ���������ش�����Ϣ
                    else
                    {
                        co_await asio::async_write(socket, asio::buffer("-error:server exception\r\n"), use_awaitable);
                        fmt::print("�������ڲ�����\n", reply.value());
                    }
                }
            }
            
        }
        //��ȡ�̶����ȵ��ֽ�
        awaitable<string> ReadFixChar(array<char, BUFSIZE>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, uint64_t length)
        {
            string line;
            line.reserve(length);
            //�жϵ�ǰ�������л��ж����ֽڿɶ�
            auto leastChar = iterEnd - iterStart;
            //���������
            if (leastChar < length)
            {
                copy(iterStart, iterEnd, back_inserter(line));
                line.resize(length);
                co_await asio::async_read(socket,asio::buffer(line.data()+leastChar,length-leastChar),use_awaitable);
                iterStart = iterEnd;
            }
            //���򣬿���һ���Զ���
            else
            {
                copy(iterStart, iterStart+length, back_inserter(line));
                iterStart += length;
            }
            co_return line;
        }

        //����һ��
        awaitable<void> SkipLine(array<char, BUFSIZE>& buf,decltype(buf.begin()) &iterStart ,decltype(buf.begin())& iterEnd)
        {
            co_await ArriveBeforeChar(buf, iterStart,iterEnd, '\n');
            co_return;
        }

        //��ȡһ��uint64_t��λ��
        awaitable<uint64_t> ReadULLAfter(array<char, BUFSIZE>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, char ch)
        {
            string lineCount;
            co_await ArriveBeforeChar(buf,iterStart,iterEnd,ch);
            decltype(buf.begin()) endIter;
            do
            {
                endIter = find(iterStart, iterEnd, '\r');
                copy(iterStart, endIter, back_inserter(lineCount));
                if (endIter != iterEnd) break;
                iterStart = buf.begin();
                iterEnd = buf.begin()+ co_await socket.async_read_some(asio::buffer(buf), use_awaitable);
            } while (true);
            iterStart = endIter;
            co_return lexical_cast<uint64_t>(lineCount);
        }

        awaitable<void> ArriveBeforeChar(array<char, BUFSIZE>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, char ch)
        {
            auto iterNow = find(iterStart, iterEnd, ch);
            while (iterNow == iterEnd)
            {
                iterEnd = buf.begin()+ co_await socket.async_read_some(asio::buffer(buf), use_awaitable);
                iterNow = find(buf.begin(), iterEnd, ch);
            }
            iterStart=iterNow+1;
        }
    };                                                                                                             
    Regist(Echo, 0);
}