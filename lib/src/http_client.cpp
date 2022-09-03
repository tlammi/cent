#include "cent/http_client.hpp"

#include <string>

#include "cent/http/header.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"
namespace cent {
namespace {

constexpr std::string_view strip(std::string_view what, char token) {
    while (!what.empty() && what.front() == token) what.remove_prefix(1);
    while (!what.empty() && what.back() == token) what.remove_suffix(1);
    return what;
}
}  // namespace

HttpClient::HttpClient(HttpSession* sess) : m_sess{sess} {}
void HttpClient::on_header(std::string_view field, std::string& value) {}

int HttpClient::get(std::string_view url) {
    std::string auth_challenge;
    m_sess->on_header("www-authenticate", auth_challenge);
    int res = m_sess->get(url);
    if (res >= 200 && res < 300) return res;
    if (res >= 400 && res < 500) {
        if (!auth_challenge.empty()) {
            logs::debug("Got challenge: \'", auth_challenge, '\'');
            http::HeaderView auth_challenge_header(auth_challenge);
            auto url = auth_challenge_header.sub_value("bearer realm");
            url = strip(url, '"');
            raise("auth challenge not implemented yet");
        }
    }
    raise("Unsupported status code '", res, '\'');
}

}  // namespace cent