#pragma once

#include "cent/drv/http_session.hpp"

namespace cent::drv {

std::unique_ptr<HttpSession> default_http_session();
void set_http_session(std::string_view str);
std::unique_ptr<HttpSession> http_session();
}  // namespace cent::drv
