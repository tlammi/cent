#include "cent/http_client.hpp"

#include <string>

#include "cent/http/header.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"
namespace cent {

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
            raise("auth challenge not implemented yet");
        }
    }
    raise("Unsupported status code '", res, '\'');
}

}  // namespace cent