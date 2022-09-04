#include "cent.hpp"

#include <cerrno>
#include <iostream>

#include "cent/http_client.hpp"
#include "cent/image.hpp"
#include "cent/logs.hpp"
#include "cent/registry_client.hpp"

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
        HttpClient http_client{m_iface};
        RegistryClient client{&http_client};
        std::stringstream ss;
        ss << client.manifest_list(Image{std::string{image}});
        return {0, ss.str()};

        // client.set_header_field(
        //     "Accept",
        //     "application/vnd.docker.distribution.manifest.list.v2+json");
        // int code = client.get(
        //     "https://registry-1.docker.io/v2/library/ubuntu/manifests/20.04");
        // return {code, std::string(client.get_body())};
    }

 private:
    Interface* m_iface;
};

Cent::Cent(Interface* iface) : m_impl{new CentImpl(iface)} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) { return m_impl->pull(image); }

}  // namespace cent