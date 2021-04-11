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
        //缓冲区大小暂定为2000个字节
        constexpr static int BUFSIZE = 2000;//缓冲区大小暂定为2000个字节
        //处理myredis网络连接（在此之前，已建立网络连接）
        virtual awaitable<void> Run()
        {
            array<char, BUFSIZE> buf;
            auto iterBegin = buf.begin(), iterEnd= buf.begin();
            //iterBegin和iterEnd是一对指针
            //iterBegin:应该从哪里开始读
            //iterEnd:读到哪里算结束

            array<string, 6> replys =//用于测试的回复
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
                //读取*号后面的正整数（一共有多少行）
                auto lineCount = co_await ReadULLAfter(buf,iterBegin, iterEnd,'*');
                vector<string> args;
                args.reserve(lineCount);
                fmt::print("line count:{}\n", lineCount);
                for (size_t i = 0; i < lineCount; ++i)
                {
                    //读取'$'号后面的正整数（一行有多长）
                    auto length = co_await ReadULLAfter(buf, iterBegin,iterEnd, '$');
                    fmt::print("line length:{}\n", length);
                    //跳过当前没读完的行
                    co_await SkipLine(buf, iterBegin, iterEnd);
                    //读取固定大小的字符，并塞入参数列表中
                    args.emplace_back(co_await ReadFixChar(buf, iterBegin, iterEnd, length));
                    fmt::print("line context:\n{}\n", args.back());
                }
                //参数的第一个字符串是函数名，查找是否有这个函数
                auto iter = getfuncManager().find(args[0]);
                //函数不存在，返回错误信息（当前为了客户端调试方便，返回一个用于测试的回复
                if (iter == getfuncManager().end())
                {
                    co_await asio::async_write(socket, asio::buffer(reply->data(), reply->size()), use_awaitable);
                    fmt::print("test reply:{}\n", *reply);
                    ++reply;
                    if (reply == replys.cend()) reply = replys.cbegin();
                }
                //函数存在，运行该函数，并将结果返回给客户端
                else
                {
                    auto reply =iter->second(std::move(args));
                    //如果reply不为空
                    if (reply.has_value())
                    {
                        co_await asio::async_write(socket, asio::buffer(reply.value().data(), reply.value().size()), use_awaitable);
                        fmt::print("reply:{}\n", reply.value());
                    }
                    //reply为空，函数运行时崩溃，返回错误信息
                    else
                    {
                        co_await asio::async_write(socket, asio::buffer("-error:server exception\r\n"), use_awaitable);
                        fmt::print("服务器内部错误\n", reply.value());
                    }
                }
            }
            
        }
        //读取固定长度的字节
        awaitable<string> ReadFixChar(array<char, BUFSIZE>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, uint64_t length)
        {
            string line;
            line.reserve(length);
            //判断当前缓冲区中还有多少字节可读
            auto leastChar = iterEnd - iterStart;
            //如果不够读
            if (leastChar < length)
            {
                copy(iterStart, iterEnd, back_inserter(line));
                line.resize(length);
                co_await asio::async_read(socket,asio::buffer(line.data()+leastChar,length-leastChar),use_awaitable);
                iterStart = iterEnd;
            }
            //否则，可以一次性读完
            else
            {
                copy(iterStart, iterStart+length, back_inserter(line));
                iterStart += length;
            }
            co_return line;
        }

        //跳过一行
        awaitable<void> SkipLine(array<char, BUFSIZE>& buf,decltype(buf.begin()) &iterStart ,decltype(buf.begin())& iterEnd)
        {
            co_await ArriveBeforeChar(buf, iterStart,iterEnd, '\n');
            co_return;
        }

        //读取一个uint64_t，位于
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