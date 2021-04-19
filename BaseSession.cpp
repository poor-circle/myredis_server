#include "stdafx.h"
#include "BaseSession.h"
#include "Func/funcManager.h"
namespace myredis
{

#include "namespace.i"
#define ReadULLAfter(ret,buf,iterStart,iterEnd,ch)\
do\
{\
    myredis::string _lineCountStr;\
    auto iterNow = find(iterStart, iterEnd, ch);\
    while (iterNow == iterEnd)\
    {\
        iterEnd = buf.begin() + co_await self->socket.async_read_some(asio::buffer(buf), use_awaitable);\
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
        iterEnd = buf.begin()+ co_await self->socket.async_read_some(asio::buffer(buf), use_awaitable);\
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
        iterEnd = buf.begin() + co_await self->socket.async_read_some(asio::buffer(buf), use_awaitable);\
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
        iterEnd = buf.begin() + co_await self->socket.async_read_some(asio::buffer(buf), use_awaitable);\
        assert((fmt::print("{}\n", fmt::join(buf.begin(), iterEnd, "")), 1));\
        _iterNow = find(buf.begin(), iterEnd, '\n');\
        copy(buf.begin(),_iterNow,back_inserter(ret));\
    }\
    ret.pop_back();\
    iterStart = _iterNow + 1;\
}while(false)

    //��ȡ�̶����ȵ��ֽ�
#define ReadFixChar(line,buf,iterStart,iterEnd,length)\
do{\
    line.clear();\
    line.reserve(length);\
    auto leastChar = (size_t)(iterEnd - iterStart);\
    if (leastChar < length)\
    {\
        copy(iterStart, iterEnd, back_inserter(line));\
        line.resize(length);\
        co_await asio::async_read(self->socket, asio::buffer(line.data() + leastChar, length - leastChar), use_awaitable);\
        iterStart = iterEnd;\
    }\
    else\
    {\
        copy(iterStart, iterStart + length, back_inserter(line));\
        iterStart += length;\
    }\
}while(false)

    BaseSession::BaseSession(asio::io_context& ioc, tcp::socket socket) :ioc(ioc), socket(move(socket)) {}

    constexpr static int BUFSIZE = 1000;//��������С�ݶ�Ϊ4000���ֽ�

    awaitable<void> BaseSession::Run(unique_ptr<BaseSession> self)
    {
        assert((fmt::print("session at thread:{}\n", std::this_thread::get_id()),1));
        assert((fmt::print
        (
            "|{0:-^{2}}|\n"
            "|{1: ^{2}}|\n"
            "|{0:-^{2}}|\n\n",
            "",
            fmt::format("connect from client {}:{}", self->socket.remote_endpoint().address(), self->socket.remote_endpoint().port()),
            75
        ), 1));
        try
        {
            array<char, BUFSIZE> buf;
            auto iterBegin = buf.begin(), iterEnd = buf.begin();
            //iterBegin��iterEnd��һ��ָ��
            //iterBegin:Ӧ�ô����￪ʼ��
            //iterEnd:�������������
            
            string errorReply={"-error:no such command\r\n"};//���ڲ��ԵĻظ�
            while (true)
            {
                //��ȡ*�ź������������һ���ж����У�
                size_t lineCount = 0;
                if (iterEnd == iterBegin)
                {
                    iterBegin = buf.begin();
                    iterEnd = iterBegin + co_await self->socket.async_read_some(asio::buffer(buf), use_awaitable);
                }

                vector<string> args;
                string line;
                if (*iterBegin == '*') //bulk string
                {
                    ReadULLAfter(lineCount, buf, iterBegin, iterEnd, '*');
                    args.reserve(lineCount);
                    assert((fmt::print("line count:{}\n", lineCount), 1));
                    for (size_t i = 0; i < lineCount; ++i)
                    {
                        //��ȡ'$'�ź������������һ���ж೤��
                        size_t length = 0;
                        ReadULLAfter(length, buf, iterBegin, iterEnd, '$');
                        assert((fmt::print("line length:{}\n", length), 1));
                        //������ǰû�������
                        SkipLine(buf, iterBegin, iterEnd);
                        //��ȡ�̶���С���ַ�������������б���
                        ReadFixChar(line, buf, iterBegin, iterEnd, length);
                        args.emplace_back(std::move(line));
                        assert((fmt::print("line context:\n{}\n", args.back()), 1));
                        SkipLine(buf, iterBegin, iterEnd);
                    }
                }
                else //single line command
                {
                    ReadLine(line, buf, iterBegin, iterEnd);
                    args.emplace_back(std::move(line));
                }

                //�����ĵ�һ���ַ����Ǻ������������Ƿ����������
                if (args.size() == 0) continue;
                boost::algorithm::to_lower(args[0]);
                auto iter = getfuncManager().find(args[0]);
                
                //���������ڣ����ش�����Ϣ����ǰΪ�˿ͻ��˵��Է��㣬����һ�����ڲ��ԵĻظ�
                if (iter == getfuncManager().end())
                {
                    co_await asio::async_write(self->socket, asio::buffer(errorReply.data(),errorReply.size()), use_awaitable);
                    assert((fmt::print("test reply:{}\n", errorReply), 1));
                }
                //�������ڣ����иú���������������ظ��ͻ���
                else
                {
                    //iter->second��һ��������ͨ��args[0]���Ҷ��ã�
                    auto reply = iter->second(func::Ctx(std::move(args), *self.get()));
                    if (!self->socket.is_open())  //��������������ӹرգ�ֱ�ӷ���
                        co_return;
                    //���reply��Ϊ��
                    if (reply.has_value())
                    {
                        co_await asio::async_write(self->socket, asio::buffer(reply.value().data(), reply.value().size()), use_awaitable);
                        assert((fmt::print("reply:{}\n", reply.value()), 1));
                    }
                    //replyΪ�գ���������ʱ���������ش�����Ϣ
                    else
                    {
                        co_await asio::async_write(self->socket, asio::buffer("-error:server exception\r\n"), use_awaitable);
                        assert((fmt::print("�������ڲ�����\n", reply.value()), 1));
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
        catch (const exception& e) 
        {
            assert((fmt::print
            (
                "*{0:*^{2}}*\n"
                "{1}\n"
                "*{0:*^{2}}*\n\n",
                "",
                fmt::format("session exception:{}", e.what()),
                75
            ), 1));
        };
        co_return;
    }

    void BaseSession::stop()
    {
        socket.shutdown(asio::socket_base::shutdown_both);
        socket.close();
    }

    int BaseSession::getDataBaseIndex()
    {
        return dataBaseIndex;
    }

    void BaseSession::setDataBaseIndex(int index)
    {

    }

}