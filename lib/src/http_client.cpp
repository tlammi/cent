#include "cent/http_client.hpp"

namespace cent {

HttpClient::HttpClient(HttpSession* sess) : m_sess{sess} {}

}  // namespace cent