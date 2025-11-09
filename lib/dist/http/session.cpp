#include <curl/curl.h>

#include <cent/dist/http/session.hpp>
#include <cent/error.hpp>

namespace cent::dist::http {

Session::Session() : m_handle(curl_easy_init()) {
    // TODO: throw
    if (!m_handle) { raise(ErrorCode::Generic, "curl_easy_init()"); }
}

Session::Session(Session&& other) noexcept
    : m_handle(std::exchange(other.m_handle, nullptr)) {}

Session& Session::operator=(Session&& other) noexcept {
    std::destroy_at(this);
    std::construct_at(this, std::move(other));
    return *this;
}

Session::~Session() { curl_easy_cleanup(m_handle); }

}  // namespace cent::dist::http
