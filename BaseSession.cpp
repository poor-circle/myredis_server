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

    //??????????????????
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
        logined(false), blocked(false), clock(ioc),watch_list(nullptr),
        isInnerSession(false)
    {
        if (strlen(myredis_password) == 0) logined = true;
        sessionID = BaseSession::IDNow++;
        getSessionMap().sessionMap.emplace(sessionID,this);
    }

    constexpr static int BUFSIZE = 1000;//????????????????1000??????

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

            //iterBegin??iterEnd??????????
            //iterBegin:????????????????
            //iterEnd:??????????????
            vector<string> args;
            while (self->closed==false)
            {
                //????*??????????????????????????????
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
                        //????'$'????????????????????????????
                        size_t length = 0;
                        ReadULLAfter(length, buf, iterBegin, iterEnd, '$');
                        assert((fmt::print("line length:{}\n", length), 1));
                        //??????????????????
                        SkipLine(buf, iterBegin, iterEnd);
                        //????????????????????????????????????
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

    int64_t BaseSession::getDataBaseID() const noexcept
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

    awaitable<optional<string>> BaseSession::wait()
    {
        try
        {
            co_await clock.async_wait(asio::use_awaitable);//????????
            //??????????????????????
            for (auto&& e : *watch_list)//??????????????????????
            {
                e.first->erase(e.second);
            }
        }
        catch (const exception& e)//????????,??????????????????????
        {
            
        }
        watch_list = nullptr;
        blocked = false;
        co_return std::move(result);
    }

    asio::awaitable<void> BaseSession::wake_up(const string& sv, int64_t dataBaseID)
    {
        static auto& set = BaseSession::getSessionMap();
        auto& watchMap = objectMap::getWatchMap(dataBaseID);
        auto iter = watchMap.find(sv);
        if (iter == watchMap.end())
            co_return;
        auto& queue = iter->second;
        if (queue.empty())
        {
            watchMap.erase(iter);
            co_return;
        }
        while (true)
        {
            auto iter2=set.find(queue.front().sessionID);//????????????????
            if (iter2 != set.end())//????????
            {
                auto ptr = iter2->second;
                //TODO:??????????????????????
                if (ptr->socket.is_open())
                {
                    auto ret = co_await(*queue.front().op)(sv);
                    if (code::isFuncSucceed(ret))
                    {
                        ptr->wake_up(std::move(ret));
                        for (auto&& e : *queue.front().nodes)//??????????????????????
                        {
                            e.first->erase(e.second);
                        }
                    }
                    else break;
                }
            }
            else
            {
                queue.front().nodes->erase(&queue);//key??????????????????????
                queue.pop_front();
            }
            if (queue.empty())
            {
                watchMap.erase(iter);//????key????????????????????key
                break;
            }
        }
        co_return;
    }

    int64_t BaseSession::getSessionID() const  noexcept
    {
        return sessionID;
    }
    //??????????????????????????????
    void BaseSession::addNewCoroToSendMessage(string&& msg)
    {
        auto& soc = socket;
        asio::co_spawn(ioc, [msg=std::move(msg),&soc]()-> asio::awaitable<void>
        {
            co_await asio::async_write(soc, asio::buffer(msg.c_str(), msg.size()), use_awaitable); 
        }, detached);
    }

    bool BaseSession::isSubscribed() const noexcept
    {
        return subChannels.size()!=0||subPatterns.size()!=0;
    }

    int64_t BaseSession::getSubscribeCount() const noexcept
    {
        return subChannels.size()+subPatterns.size();
    }

    bool BaseSession::getIsInnerSession()
    {
        return isInnerSession;
    }

    void BaseSession::setIsInnerSession(bool flag)
    {
        isInnerSession = flag;
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

    hash_map<string, hash_set<int64_t>>& BaseSession::getChannelMap() noexcept
    {
        static hash_map<string, hash_set<int64_t>> channelMap;
        return channelMap;
    }
    hash_map<Pattern, hash_set<int64_t>>& BaseSession::getPatternTable() noexcept
    {
        static hash_map<Pattern, hash_set<int64_t>> patternTable;
        return patternTable;
    }
    hash_set<string>& BaseSession::getsubChannels() noexcept
    {
        return subChannels;
    }

    hash_set<Pattern>& BaseSession::getsubPatterns() noexcept
    {
        return subPatterns;
    }

    void BaseSession::wake_up(optional<string>&& result)
    {
        this->result = std::move(result);
        clock.cancel_one();
    }

    BaseSession::~BaseSession()
    {
        // ????????
        auto& channelMap = BaseSession::getChannelMap();
        auto& subChannels = getsubChannels();
        for(auto& e:subChannels)
        {
            auto iter = channelMap.find(e);
            iter->second.erase(sessionID);
            if (iter->second.empty()) {
                channelMap.erase(iter);
            }
        }

        // ????????
        auto& patternTable = BaseSession::getPatternTable();
        auto& subPatterns = getsubPatterns();
        for (auto& e : subPatterns)
        {
            auto iter = patternTable.find(e);
            iter->second.erase(sessionID);
            if (iter->second.empty()) {
                patternTable.erase(iter);
            }
        }

        getSessionMap().sessionMap.erase(sessionID);
    }
    atomic<int64_t> BaseSession::IDNow=0;
}