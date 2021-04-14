#include "stdafx.h"
#include "Session.h"
#include "object.hpp"
#include "Func/funcManager.h"
namespace myredis
{
#define ReadULLAfter(ret,buf,iterStart,iterEnd,ch)\
do\
{\
    myredis::string _lineCountStr;\
    auto iterNow = find(iterStart, iterEnd, ch);\
    while (iterNow == iterEnd)\
    {\
        iterEnd = buf.begin() + co_await socket.async_read_some(asio::buffer(buf), use_awaitable);\
        assert((fmt::print("{}\n", fmt::join(buf.begin(), iterEnd, "")), 1));\
        iterNow = find(buf.begin(), iterEnd, ch);\
    }\
    iterStart = iterNow + 1;\
    decltype(buf.begin()) endIter;\
    do\
    {\
        endIter = find(iterStart, iterEnd, '\r');\
        copy(iterStart, endIter, back_inserter(_lineCountStr));\
        if (endIter != iterEnd) break;\
        iterStart = buf.begin();\
        iterEnd = buf.begin()+ co_await socket.async_read_some(asio::buffer(buf), use_awaitable);\
    } while (true);\
    iterStart = endIter;\
    ret=lexical_cast<size_t>(_lineCountStr);\
}while(false)


#define SkipLine(buf,iterStart ,iterEnd)\
do\
{\
    auto _iterNow = find(iterStart, iterEnd, '\n');\
    while (_iterNow == iterEnd)\
    {\
        iterEnd = buf.begin() + co_await socket.async_read_some(asio::buffer(buf), use_awaitable);\
        assert((fmt::print("{}\n", fmt::join(buf.begin(), iterEnd, "")), 1));\
        _iterNow = find(buf.begin(), iterEnd, '\n');\
    }\
    iterStart = _iterNow + 1;\
}while(false)


#define ReadLine(ret,buf,iterStart ,iterEnd)\
do\
{\
    ret.clear();\
    auto _iterNow = find(iterStart, iterEnd, '\n');\
    copy(iterStart,_iterNow,back_inserter(ret));\
    while (_iterNow == iterEnd)\
    {\
        iterEnd = buf.begin() + co_await socket.async_read_some(asio::buffer(buf), use_awaitable);\
        assert((fmt::print("{}\n", fmt::join(buf.begin(), iterEnd, "")), 1));\
        _iterNow = find(buf.begin(), iterEnd, '\n');\
        copy(buf.begin(),_iterNow,back_inserter(ret));\
    }\
    ret.pop_back();\
    iterStart = _iterNow + 1;\
}while(false)

//读取固定长度的字节
#define ReadFixChar(line,buf,iterStart,iterEnd,length)\
do{\
    line.clear();\
    line.reserve(length);\
    auto leastChar = iterEnd - iterStart;\
    if (leastChar < length)\
    {\
        copy(iterStart, iterEnd, back_inserter(line));\
        line.resize(length);\
        co_await asio::async_read(socket, asio::buffer(line.data() + leastChar, length - leastChar), use_awaitable);\
        iterStart = iterEnd;\
    }\
    else\
    {\
        copy(iterStart, iterStart + length, back_inserter(line));\
        iterStart += length;\
    }\
}while(false)

    
#include"namespace.h"
#ifdef QPSTEST
    uint64_t cnter = 0;
    high_resolution_clock::time_point tp;
#endif
    class Echo :Session
    {
    public:
        static uint32_t sessionTypeCode;
        Echo(asio::io_context& ioc, tcp::socket socket) :Session(ioc, std::move(socket)) {};
        //缓冲区大小暂定为2000个字节
        constexpr static int BUFSIZE = 1000;//缓冲区大小暂定为4000个字节
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
                size_t lineCount = 0;
                if (iterEnd == iterBegin)
                {
                    iterBegin = buf.begin();
                    iterEnd = iterBegin + co_await socket.async_read_some(asio::buffer(buf), use_awaitable);
                }
                    
                vector<string> args;
                string line;
                if (*iterBegin == '*') //bulk string
                {
                    ReadULLAfter(lineCount,buf, iterBegin, iterEnd, '*');
                    args.reserve(lineCount);
                    assert((fmt::print("line count:{}\n", lineCount), 1));
                    for (size_t i = 0; i < lineCount; ++i)
                    {
                        //读取'$'号后面的正整数（一行有多长）
                        size_t length = 0;
                        ReadULLAfter(length, buf, iterBegin, iterEnd, '$');
                        assert((fmt::print("line length:{}\n", length), 1));
                        //跳过当前没读完的行
                        SkipLine(buf, iterBegin, iterEnd);
                        //读取固定大小的字符，并塞入参数列表中
                        ReadFixChar(line, buf, iterBegin, iterEnd, length);
                        args.emplace_back(std::move(line));
                        assert((fmt::print("line context:\n{}\n", args.back()), 1));
                        SkipLine(buf, iterBegin, iterEnd);
                    }
                }
                else //single line command
                {
                    ReadLine(line,buf,iterBegin,iterEnd);
                    args.emplace_back(std::move(line));
                }
                
                //参数的第一个字符串是函数名，查找是否有这个函数
                if (args.size() == 0) continue;
                boost::algorithm::to_lower(args[0]);
                auto iter = getfuncManager().find(args[0]);
                //函数不存在，返回错误信息（当前为了客户端调试方便，返回一个用于测试的回复
                if (iter == getfuncManager().end())
                {
                    co_await asio::async_write(socket, asio::buffer(reply->data(), reply->size()), use_awaitable);
                    assert((fmt::print("test reply:{}\n", *reply), 1));
                    ++reply;
                    if (reply == replys.cend()) reply = replys.cbegin();
                }
                    
                //函数存在，运行该函数，并将结果返回给客户端
                else
                {
                    //iter->second是一个函数（通过args[0]查找而得）
                    auto reply = iter->second(std::move(args));
                    //如果reply不为空
                    if (reply.has_value())
                    {
                        co_await asio::async_write(socket, asio::buffer(reply.value().data(), reply.value().size()), use_awaitable);
                        assert((fmt::print("reply:{}\n", reply.value()), 1));
                    }
                    //reply为空，函数运行时崩溃，返回错误信息
                    else
                    {
                        co_await asio::async_write(socket, asio::buffer("-error:server exception\r\n"), use_awaitable);
                        assert((fmt::print("服务器内部错误\n", reply.value()), 1));
                    }
                }
#ifdef QPSTEST
                ++cnter;
                if (cnter % 100000 == 0)
                {
                    auto now = high_resolution_clock::now();
                    fmt::print("QPS:{}\n", 100000000 / duration_cast<milliseconds>(now - tp).count());
                    tp = high_resolution_clock::now();
                }
#endif    
            }
         
        }
        


        //awaitable<void> ArriveBeforeChar(array<char, BUFSIZE>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, char ch)
        //{
        //    auto iterNow = find(iterStart, iterEnd, ch);
        //    while (iterNow == iterEnd)
        //    {
        //        iterEnd = buf.begin()+ co_await socket.async_read_some(asio::buffer(buf), use_awaitable);
        //        assert((fmt::print("{}\n", fmt::join(buf.begin(), iterEnd, "")),1));
        //        iterNow = find(buf.begin(), iterEnd, ch);
        //    }
        //    iterStart=iterNow+1;
        //}
    };                                                                                                             
    Regist(Echo, 0);
}