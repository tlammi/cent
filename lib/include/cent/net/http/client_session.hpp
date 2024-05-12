#pragma once

#include <cent/net/http/response.hpp>
#include <cent/net/url.hpp>

namespace cent::net::http {

class ClientSession {
 public:
    constexpr ClientSession() noexcept = default;

    Response get(UrlView url);

 private:
};

}  // namespace cent::net::http
