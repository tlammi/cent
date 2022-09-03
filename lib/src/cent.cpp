#include "cent.hpp"

#include <cerrno>
#include <iostream>

#include "cent/http_client.hpp"
#include "cent/logs.hpp"

namespace cent {

/**
 * Log consuming stream
 *
 * Set this to change where logs are written
 */
std::reference_wrapper<std::ostream> LOG_STREAM{std::cerr};

class Cent::CentImpl {
 public:
    CentImpl(Interface* iface) : m_iface{iface} {}

    Result pull(std::string_view image) {
        HttpClient client{m_iface->http_session()};
        int code = client.get(
            "https://registry-1.docker.io/v2/library/ubuntu/manifests/20.04");
        return {code, "foobar"};
    }

 private:
    Interface* m_iface;
};

Cent::Cent(Interface* iface) : m_impl{new CentImpl(iface)} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) { return m_impl->pull(image); }

}  // namespace cent