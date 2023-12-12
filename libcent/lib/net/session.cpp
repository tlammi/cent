
#include <cent/bits/net/session.hpp>

namespace cent::net {

Session::Session() : m_c(curl_easy_init()) {}
Session::~Session() { curl_easy_cleanup(m_c); }

void Session::on_header_impl(size_t (*cb)(char*, size_t, size_t, void*),
                             void* userdata) noexcept {
    curl_easy_setopt(m_c, CURLOPT_HEADERFUNCTION, cb);
    curl_easy_setopt(m_c, CURLOPT_HEADERDATA, userdata);
}

void Session::on_write_impl(size_t (*cb)(char*, size_t, size_t, void*),
                            void* userdata) noexcept {
    curl_easy_setopt(m_c, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(m_c, CURLOPT_WRITEDATA, userdata);
}

void Session::url(const char* s) { curl_easy_setopt(m_c, CURLOPT_URL, s); }
net::Result Session::get() {
    curl_easy_perform(m_c);
    return {};
}

}  // namespace cent::net
