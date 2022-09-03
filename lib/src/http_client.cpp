#include "cent/http_client.hpp"

namespace cent {

HttpClient::HttpClient(HttpSession* sess) : m_sess{sess} {}
int HttpClient::get(std::string_view url) { return m_sess->get(url); }

}  // namespace cent