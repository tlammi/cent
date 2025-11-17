#pragma once

#include <cent/data/image_index.hpp>
#include <cent/data/manifest.hpp>
#include <cent/dist/http/session.hpp>

namespace cent::dist {

class RegistryClient {
 public:
    explicit RegistryClient(http::AnySession& sess) noexcept;

    std::variant<data::ImageIdx, data::Manifest> manifest(UrlView url);

 private:
    http::AnySession* m_sess;
};
}  // namespace cent::dist
