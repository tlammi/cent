#pragma once

#include <curl/curl.h>

#include <string>
#include <utility>

namespace cent {

class CurlSlist {
 public:
    constexpr CurlSlist() noexcept = default;
    CurlSlist(const CurlSlist&) = delete;
    CurlSlist& operator=(const CurlSlist&) = delete;

    CurlSlist(CurlSlist&& other) noexcept;
    CurlSlist& operator=(CurlSlist&& other) noexcept;

    ~CurlSlist();

    void append(const char* s);
    void append(const std::string& s) { append(s.c_str()); }

    curl_slist* handle() const noexcept;

    void clear() noexcept;

 private:
    curl_slist* m_front{nullptr};
    curl_slist* m_back{nullptr};  // for faster insert
};
}  // namespace cent
