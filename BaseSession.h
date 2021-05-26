#pragma once
#include "stdafx.h"
#include "object.hpp"

//�����ܿ⣬����һ�����������ӻỰ
namespace myredis
{

    class BaseSession;

    class SessionMap
    {
        friend class BaseSession;
        hash_map<int64_t, BaseSession*> sessionMap;

        SessionMap(const SessionMap&) = delete;
        SessionMap(SessionMap&&) = delete;
        SessionMap& operator = (SessionMap&&) = delete;
        SessionMap& operator = (const SessionMap&) = delete;
        SessionMap() = default;
    public:
        decltype(auto) begin() { return sessionMap.begin(); }
        decltype(auto) find(int64_t sessionID) { return sessionMap.find(sessionID); }
        decltype(auto) end() { return sessionMap.end(); }
        decltype(auto) size() { return sessionMap.size(); }
    };

    class BaseSession
    {
        friend struct std::default_delete<myredis::BaseSession>;
    public:
        
        BaseSession(const BaseSession&) = delete;
        BaseSession(BaseSession&&) = delete;
        BaseSession& operator =(const BaseSession&) = delete;
        BaseSession& operator =(BaseSession&&) = delete;
        static std::unique_ptr<BaseSession> create(asio::io_context& ioc, asio::ip::tcp::socket&& socket);


        static asio::awaitable<void> run(std::unique_ptr<BaseSession> self);
        void setDataBaseID(int64_t ID)  noexcept;
        int64_t getDataBaseID() const noexcept;
        objectMap& getObjectMap() noexcept;

        void setClosed() noexcept;
        bool isLogined() const noexcept;
        void setLogined(bool logined) noexcept;
        static SessionMap& getSessionMap();
        // ���ȫ�ֵ�Ƶ����
        static hash_map<string, hash_set<int64_t>>& getChannelMap() noexcept;
        static hash_map<Pattern,hash_set<int64_t>>& getPatternTable() noexcept;
        // ���ĵ�Ƶ��
        hash_set<string>& getsubChannels() noexcept;
        hash_set<Pattern>& getsubPatterns() noexcept;
        bool isBlocked() const noexcept;
        void setBlocked(string time_out_reply, std::chrono::steady_clock::duration time,watcherPtr& watch_list);
        asio::awaitable<std::optional<string>> wait();
        static asio::awaitable<void> wake_up(const string& sv, int64_t dataBaseID);
        std::queue<std::pair<string, int64_t>> wake_up_queue;
        int64_t getSessionID() const noexcept;
        //����һ���µ�Э��������һ����Ϣ
        void addNewCoroToSendMessage(string&& msg);
        bool isSubscribed() const noexcept;
        int64_t getSubscribeCount() const noexcept;
        bool getIsInnerSession();
        void setIsInnerSession(bool);
    private:
        ~BaseSession();
        BaseSession(asio::io_context& ioc, asio::ip::tcp::socket&& socket);
        bool closed;
        bool logined;
        bool blocked;
        bool isInnerSession;
        int64_t dataBaseID;
        int64_t sessionID;
        // ���ڼ�¼��ǰ�Ự�����˶��ٸ�Ƶ��
        hash_set<string> subChannels;
        hash_set<Pattern> subPatterns;
        static std::atomic<int64_t> IDNow;
        void wake_up(std::optional<string>&& result);
        std::optional<string> result;
        asio::io_context& ioc;
        asio::ip::tcp::socket socket;
        asio::steady_timer clock;
        std::shared_ptr<hash_map<boost::container::list<watchInfo>*, boost::container::list<watchInfo>::iterator>> watch_list;
        
    };
    
}