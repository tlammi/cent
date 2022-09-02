#pragma once

#include "cent.hpp"
#include "http_session.hpp"

class Interface final : public cent::Interface {
 public:
  cent::HttpSession* http_session() { return &m_tmp; }

 private:
  HttpSession m_tmp{};
};
