#pragma once

#include <cent/net/http_session.hpp>
#include <memory>

namespace cent::net {

std::unique_ptr<HttpSession> http_session();
}