#pragma once

#include <list>

#include "dist/http/session.hpp"

namespace cent::dist::http {

class SessionPool {
    struct Deleter {
        SessionPool* pool;
        void operator()(AnySession* sess) { pool->free_session(sess); }
    };

 public:
    using Ptr = std::unique_ptr<AnySession, Deleter>;

    Ptr session() { return Ptr{get_session(), Deleter{.pool = this}}; }

 protected:
    ~SessionPool() = default;

 private:
    virtual AnySession* get_session() = 0;
    virtual void free_session(AnySession* sess) = 0;
};

// Trivial placeholder implementation
class SimpleSessionPool final : public SessionPool {
 public:
    ~SimpleSessionPool() = default;

 private:
    AnySession* get_session() override {
        m_sessions.push_back(Session());
        return &m_sessions.back();
    }

    void free_session(AnySession* sess) override {
        auto it = std::ranges::find_if(m_sessions, [&](const auto& elem) {
            return std::addressof(elem) == sess;
        });
        m_sessions.erase(it);
    }

    std::list<Session> m_sessions{};
};

}  // namespace cent::dist::http
