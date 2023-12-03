
#pragma once

#include <curl/curl.h>

#include <cent/concepts.hpp>
#include <cent/http/curl_slist.hpp>
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

    void clear_headers_impl() override;
    void add_header_impl(std::string_view key, std::string_view value) override;
    void commit_headers_impl() override;

    Result get_impl(const char* url) override;

    CURL* m_c{};
    CurlSList m_headers{};
};
}  // namespace cent::http
