#include <fmt/format.h>

#include <cent/http/curl_session.hpp>
#include <cent/util.hpp>

namespace cent::http {
namespace {

template <class... Ts>
void do_curl_easy_setopt(CURL* hndl, CURLoption opt, Ts&&... ts) {
    auto res = curl_easy_setopt(hndl, opt, std::forward<Ts>(ts)...);
    CENT_CHECK(res == CURLE_OK);
}

template <class... Ts>
void do_curl_easy_getinfo(CURL* hndl, CURLINFO info, Ts&&... ts) {
    auto res = curl_easy_getinfo(hndl, info, std::forward<Ts>(ts)...);
    CENT_CHECK(res == CURLE_OK);
}
}  // namespace

CurlSession::CurlSession() : m_c(curl_easy_init()) { CENT_CHECK(m_c); }

CurlSession::CurlSession(CurlSession&& other) noexcept {
    std::swap(m_c, other.m_c);
}

CurlSession& CurlSession::operator=(CurlSession&& other) noexcept {
    if (this != &other) {
        curl_easy_cleanup(m_c);
        m_c = nullptr;
        std::swap(m_c, other.m_c);
    }
    return *this;
}

CurlSession::~CurlSession() { curl_easy_cleanup(m_c); }

void CurlSession::on_write_impl(size_t (*cb)(char*, size_t, size_t, void*),
                                void* userdata) {
    do_curl_easy_setopt(m_c, CURLOPT_WRITEFUNCTION, cb);
    do_curl_easy_setopt(m_c, CURLOPT_WRITEDATA, userdata);
}

void CurlSession::on_header_impl(size_t (*cb)(char*, size_t, size_t, void*),
                                 void* userdata) {
    do_curl_easy_setopt(m_c, CURLOPT_HEADERFUNCTION, cb);
    do_curl_easy_setopt(m_c, CURLOPT_HEADERDATA, userdata);
}

void CurlSession::clear_headers_impl() { m_headers.clear(); }
void CurlSession::add_header_impl(std::string_view key,
                                  std::string_view value) {
    m_headers.append(fmt::format("{}:{}", key, value).c_str());
}
void CurlSession::commit_headers_impl() {
    do_curl_easy_setopt(m_c, CURLOPT_HTTPHEADER, m_headers.handle());
}

Result CurlSession::get_impl(const char* url) {
    do_curl_easy_setopt(m_c, CURLOPT_URL, url);
    auto res = curl_easy_perform(m_c);
    if (res == CURLE_OK) {
        long code{};
        do_curl_easy_getinfo(m_c, CURLINFO_RESPONSE_CODE, &code);
        return int_cast<Status>(code);
    } else if (res == CURLE_WRITE_ERROR) {
        return ErrorCode::Cancelled;
    } else {
        panic("Unsupported error: ", curl_easy_strerror(res));
    }
}

}  // namespace cent::http
