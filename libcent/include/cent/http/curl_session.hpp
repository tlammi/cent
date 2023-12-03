
#pragma once

#include <curl/curl.h>

#include <cent/concepts.hpp>
#include <cent/http/session.hpp>

namespace cent::http {

class CurlSession final : public Session {
 public:
    CurlSession();
    CurlSession(const CurlSession&) = delete;
    CurlSession& operator=(const CurlSession&) = delete;

    CurlSession(CurlSession&& other) noexcept;
    CurlSession& operator=(CurlSession&& other) noexcept;
    ~CurlSession() override;

 private:
    void on_write_impl(size_t (*cb)(char*, size_t, size_t, void*),
                       void* userdata) override;
    void on_header_impl(size_t (*cb)(char*, size_t, size_t, void*),
                        void* userdata) override;

    Result get_impl(const char* url) override;

    CURL* m_c{};
};
}  // namespace cent::http
