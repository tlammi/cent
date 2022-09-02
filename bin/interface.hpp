#pragma once

#include "cent.hpp"

class Interface final : public cent::Interface {
 public:
    cent::HttpSession* http_session() { return m_tmp.get(); }

 private:
    std::unique_ptr<cent::HttpSession> m_tmp{cent::default_http_session()};
};
