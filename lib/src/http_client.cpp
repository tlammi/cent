#include "cent/http_client.hpp"

#include <string>

#include "cent/logs.hpp"
namespace cent {

HttpClient::HttpClient(HttpSession* sess) : m_sess{sess} {}
void HttpClient::on_header(std::string_view field, std::string& value) {}

int HttpClient::get(std::string_view url) {
    std::string auth_challenge;
    m_sess->on_header("www-authenticate", auth_challenge);
    int res = m_sess->get(url);
    logs::debug("Got challenge: \'", auth_challenge, '\'');
    return res;
}

}  // namespace cent