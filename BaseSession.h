#pragma once
#include "stdafx.h"
#include "object.hpp"
//网络框架库，代表一个基本的连接会话
namespace myredis
{

    class BaseSession;

    class SessionMap
    {
        friend class BaseSession;
        hash_map<size_t, BaseSession*> sessionMap;
        SessionMap(const SessionMap&) = delete;
        SessionMap(SessionMap&&) = delete;
        SessionMap& operator = (SessionMap&&) = delete;
        SessionMap& operator = (const SessionMap&) = delete;
        SessionMap() = default;
    public:
        decltype(auto) begin() { return sessionMap.begin(); }
        decltype(auto) find(size_t sessionID) { return sessionMap.find(sessionID); }
        decltype(auto) end() { return sessionMap.end(); }
        decltype(auto) size() { return sessionMap.size(); }
    };

    class BaseSession
    {
    public:
        BaseSession(asio::io_context& ioc, asio::ip::tcp::socket socket);
        static asio::awaitable<void> run(std::unique_ptr<BaseSession> self);
        void setDataBaseID(int64_t ID)  noexcept;
        size_t getDataBaseID() noexcept;
        objectMap& getObjectMap() noexcept;
        void setClosed() noexcept;
        bool isLogined() noexcept;
        void setLogined(bool logined) noexcept;
        BaseSession(const BaseSession&) = delete;
        BaseSession(BaseSession&&) = delete;
        BaseSession& operator =(BaseSession&&) = delete;
        BaseSession& operator =(const BaseSession&) = delete;
        static SessionMap& getSessionMap();
        ~BaseSession();
        bool isBlocked() noexcept;
        void setBlocked(string time_out_reply, std::chrono::steady_clock::duration time,watcherPtr& watch_list);
        asio::awaitable<string> wait();
        static void wake_up(const string& sv, size_t dataBaseID);
        std::queue<std::pair<string, size_t>> wake_up_queue;
        size_t getSessionID() noexcept;
        asio::ip::tcp::socket& getSocket() noexcept;
    private:
        static std::atomic<size_t> IDNow;
        void wake_up(string&& result);
        string result;
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        asio::steady_timer clock;
        int64_t dataBaseID;
        bool closed;
        bool logined;
        bool blocked;
        size_t sessionID;
        std::shared_ptr<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>> watch_list;
    };
    
}