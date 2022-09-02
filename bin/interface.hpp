#pragma once

#include "cent.hpp"
#include "http_session.hpp"
#include "log_consumer.hpp"

class Interface final : public cent::Interface {
 public:
    cent::HttpSession* http_session() { return &m_tmp; }
    cent::LogConsumer* log_consumer() { return &m_log; }

 private:
    HttpSession m_tmp{};
    LogConsumer m_log{};
};
