
#pragma once

#include <curl/curl.h>

#include <cent/concepts.hpp>
#include <cent/http/status.hpp>

namespace cent::http {

class CurlSession {
 public:
    CurlSession();
    CurlSession(const CurlSession&) = delete;
    CurlSession& operator=(const CurlSession&) = delete;

    CurlSession(CurlSession&& other) noexcept;
    CurlSession& operator=(CurlSession&& other) noexcept;
    ~CurlSession();

    Status get();

 private:
    CURL* m_c{};
};
}  // namespace cent::http
