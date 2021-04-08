#include"stdafx.h"
#include"Session.h"
namespace myredis
{
#include"namespace.h"
    class Echo :Session
    {
    public:
        static uint32_t sessionTypeCode;
        Echo(asio::io_context& ioc, tcp::socket socket) :Session(ioc, std::move(socket)) {};

        virtual awaitable<void> Run()
        {
            array<char,1024> buf;
            auto iterBegin = buf.begin(), iterEnd= buf.begin();
            array<string, 5> replys =
            {
                "+OK\r\n",
                "-error:no such key\r\n",
                ":4213425",
                "$6\r\nfoobar\r\n",
                "*4\r\n$3\r\nfoo\r\n$3\r\nbar\r\n$5\r\nHello\r\n$5\r\nWorld"
            };
            auto reply = replys.cbegin();
            while (true)
            {
                auto lineCount = co_await ReadULLAfter(buf,iterBegin, iterEnd,'*');
                fmt::print("line count:{}\n", lineCount);
                for (size_t i = 0; i < lineCount; ++i)
                {
                    auto length = co_await ReadULLAfter(buf, iterBegin,iterEnd, '$');
                    fmt::print("line length:{}\n", length);
                    co_await SkipLine(buf, iterBegin, iterEnd);
                    auto line = co_await ReadFixChar(buf, iterBegin, iterEnd, length);
                    fmt::print("line context:{}\n", line);
                }
                co_await asio::async_write(socket, asio::buffer(reply->data(),reply->size()), use_awaitable);
                fmt::print("test reply:{}\n", *reply);
                ++reply;
                if (reply == replys.cend()) reply == replys.end();
            }
            
        }

        awaitable<string> ReadFixChar(array<char, 1024>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, uint64_t length)
        {
            string line;
            line.reserve(length);
            auto leastChar = iterEnd - iterStart;
            if (leastChar < length)
            {
                copy(iterStart, iterEnd, back_inserter(line));
                line.resize(length);
                co_await asio::async_read(socket,asio::buffer(line.data()+leastChar,length-leastChar),use_awaitable);
                iterStart = iterEnd;
            }
            else
            {
                copy(iterStart, iterStart+length, back_inserter(line));
                iterStart += length;
            }
            co_return line;
        }


        awaitable<void> SkipLine(array<char, 1024>& buf,decltype(buf.begin()) &iterStart ,decltype(buf.begin())& iterEnd)
        {
            co_await ArriveBeforeChar(buf, iterStart,iterEnd, '\n');
            co_return;
        }

        awaitable<uint64_t> ReadULLAfter(array<char, 1024>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, char ch)
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
                fmt::print("debug:{}\n", string_view(buf.data(),iterEnd-iterStart));
            } while (true);
            iterStart = endIter;
            co_return stoull(lineCount);
        }

        awaitable<void> ArriveBeforeChar(array<char, 1024>& buf, decltype(buf.begin())& iterStart, decltype(buf.begin())& iterEnd, char ch)
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