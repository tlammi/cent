#include <cent/reg/registry.hpp>

#include "cent/core/http_client.hpp"

namespace cent::reg {
Registry::Registry(std::string base_url)
    : Registry(std::move(base_url), cent::http_request()) {}

Registry::Registry(std::string base_url, Ptr<HttpRequest> req)
    : m_url{std::move(base_url)}, m_req{std::move(req)} {}

const HttpRequest* Registry::active_request() const { return m_req.get(); }

}  // namespace cent::reg
