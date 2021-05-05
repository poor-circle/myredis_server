#include "stdafx.h"
#include "BaseSession.h"
#include "Func/funcManager.h"
#include "funcController.h"
#include "code.h"
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

    //读取固定长度的字节
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

    BaseSession::BaseSession(asio::io_context& ioc, tcp::socket&& socket) :
        ioc(ioc), socket(move(socket)), dataBaseID(0), closed(false),
        logined(false), blocked(false), clock(ioc),watch_list(nullptr)
    {
        if (strlen(myredis_password) == 0) logined = true;
        sessionID = BaseSession::IDNow++;
        getSessionMap().sessionMap.emplace(sessionID,this);
    }

    constexpr static int BUFSIZE = 1000;//缓冲区大小暂定为1000个字节

    std::unique_ptr<BaseSession> BaseSession::create(asio::io_context& ioc, asio::ip::tcp::socket&& socket)
    {
        return unique_ptr<BaseSession>(new BaseSession(ioc,std::move(socket)));
    }

#ifdef QPSTEST
    static inline int64_t cnter = 0;
    static inline auto tp = high_resolution_clock::now();
#endif
    
    awaitable<void> BaseSession::run(unique_ptr<BaseSession> self)
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

            //iterBegin和iterEnd是一对指针
            //iterBegin:应该从哪里开始读
            //iterEnd:读到哪里算结束
            vector<string> args;
            while (self->closed==false)
            {
                //读取*号后面的正整数（一共有多少行）
                size_t lineCount = 0;
                if (iterEnd == iterBegin)
                {
                    iterBegin = buf.begin();
                    iterEnd = iterBegin + co_await self->socket.async_read_some(asio::buffer(buf), use_awaitable);
                }
                vector<string> args;
                args.reserve(5);
                string line;
                if (*iterBegin == '*') //bulk string
                {
                    ReadULLAfter(lineCount, buf, iterBegin, iterEnd, '*');
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
                    ReadLine(line, buf, iterBegin, iterEnd);
                    args.emplace_back(std::move(line));
                }
                string reply;
                co_await funcControll(func::context(std::move(args), *self.get()),reply);
                co_await asio::async_write(self->socket, asio::buffer(reply.data(), reply.size()), use_awaitable);
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

    void BaseSession::setDataBaseID(int64_t ID) noexcept
    {
        dataBaseID = ID;
    }

    size_t BaseSession::getDataBaseID() const noexcept
    {
        return dataBaseID;
    }

    objectMap& BaseSession::getObjectMap() noexcept
    {
        return objectMap::getObjectMap(dataBaseID);
    }



    void BaseSession::setClosed() noexcept
    {
        closed = true;
    }

    bool BaseSession::isLogined() const noexcept
    {
        return logined;
    }

    void BaseSession::setLogined(bool logined) noexcept
    {
        this->logined = logined;
    }

    awaitable<string> BaseSession::wait()
    {
        try
        {
            co_await clock.async_wait(asio::use_awaitable);//等待超时
            //超时，删除所有的监听器
            for (auto&& e : *watch_list)//删除监视表上的全部节点
            {
                e.first->erase(e.second);
            }
        }
        catch (const exception& e)//抛出异常,代表阻塞操作被事件中断
        {

        }
        watch_list = nullptr;
        blocked = false;
        co_return std::move(result);
    }

    void BaseSession::wake_up(const string& sv, size_t dataBaseID)
    {
        static auto& set = BaseSession::getSessionMap();
        auto& watchMap = objectMap::getWatchMap(dataBaseID);
        auto iter = watchMap.find(sv);
        if (iter == watchMap.end())
            return;
        auto& queue = iter->second;
        if (queue.empty())
        {
            watchMap.erase(iter);
            return;
        }
        while (true)
        {
            auto iter2=set.find(queue.front().sessionID);//查找会话是否存在
            if (iter2 != set.end())//如果存在
            {
                auto ptr = iter2->second;
                auto ret = (*queue.front().op)(sv);
                if (ret.has_value())
                {
                    ptr->wake_up(std::move(ret.value()));
                    for (auto&& e : *queue.front().nodes)//删除监视表上的全部节点
                    {
                        e.first->erase(e.second);
                    }
                }
                else break;
            }
            else
            {
                queue.front().nodes->erase(&queue);//key无效，删除对应的监视器
                queue.pop_front();
            }
            if (queue.empty())
            {
                watchMap.erase(iter);//如果key的监视节点清零，清空key
                break;
            }
        }
    }

    size_t BaseSession::getSessionID() const  noexcept
    {
        return sessionID;
    }
    //创建一个新的协程来发送一条消息
    void BaseSession::addNewCoroToSendMessage(string&& msg)
    {
        auto& soc = socket;
        asio::co_spawn(ioc, [msg=std::move(msg),&soc]()-> asio::awaitable<void>
        {
            co_await asio::async_write(soc, asio::buffer(msg.c_str(), msg.size()), use_awaitable); 
        }, detached);
    }

    bool BaseSession::isBlocked() const noexcept
    {
        return blocked;
    }

    void BaseSession::setBlocked(string time_out_reply, std::chrono::steady_clock::duration time,watcherPtr& watch_list)
    {
        result = std::move(time_out_reply);
        clock.expires_after(time);
        blocked = true;
        this->watch_list = watch_list;
    }
    SessionMap& BaseSession::getSessionMap()
    {
        static SessionMap table;
        return table;
    }

    hash_map<string, hash_set<size_t>>& BaseSession::getChannelMap() noexcept
    {
        static hash_map<string, hash_set<size_t>> channelMap;
        return channelMap;
    }

    hash_set<string>& BaseSession::getsubChannels() noexcept
    {
        return subChannels;
    }

    void BaseSession::wake_up(string&& result)
    {
        this->result = std::move(result);
        clock.cancel_one();
    }

    BaseSession::~BaseSession()
    {
        auto& channelMap = getChannelMap();
        auto& subChannels = getsubChannels();
        for(auto& e:subChannels)
        {
            auto iter = channelMap.find(e);
            if (iter != channelMap.end()) {
                iter->second.erase(sessionID);
            }
        }
        getSessionMap().sessionMap.erase(sessionID);
    }
    atomic<size_t> BaseSession::IDNow=0;
}