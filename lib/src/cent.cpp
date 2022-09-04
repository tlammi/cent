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

    Result pull(std::string_view image_ref) {
        Image image{std::string(image_ref)};
        HttpClient http_client{m_iface};
        RegistryClient client{&http_client};
        std::stringstream ss;
        auto manifest_list = client.manifest_list(image);
        auto entry =
            manifest_list.platform_manifest(Platform{"amd64", "linux"});
        if (!entry) {
            logs::debug(manifest_list);
            raise("no entry: 'amd64'");
        }
        logs::debug(manifest_list);
        std::string manifest_img_ref{image.repo()};
        manifest_img_ref += "@";
        manifest_img_ref += entry->digest.str();
        Image manifest_image{manifest_img_ref};
        return {0, client.manifest(manifest_image)};
    }

 private:
    Interface* m_iface;
};

Cent::Cent(Interface* iface) : m_impl{new CentImpl(iface)} {}
Cent::~Cent() {}

Result Cent::pull(std::string_view image) { return m_impl->pull(image); }

}  // namespace cent