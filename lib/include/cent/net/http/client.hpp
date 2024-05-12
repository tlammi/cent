#pragma once

#include <cent/net/http/response.hpp>
#include <cent/net/url.hpp>

namespace cent::net::http {

class Client {
 public:
    constexpr Client() noexcept = default;
    constexpr virtual ~Client() = default;

    Response get(UrlView url);

 private:
};

}  // namespace cent::net::http
