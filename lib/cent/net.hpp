#pragma once

#include <cent/net/http_session.hpp>
#include <memory>

#include "cent/net/http_client.hpp"

namespace cent::net {

std::unique_ptr<HttpSession> http_session();
std::unique_ptr<HttpClient> http_client();
}  // namespace cent::net