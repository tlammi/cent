#pragma once

#include <cent/data/image_index.hpp>
#include <cent/dist/http/session.hpp>

namespace cent::dist {

class RegistryClient {
 public:
    explicit RegistryClient(http::AnySession& sess) noexcept;

    data::ImageIdx manifest(http::UrlView url);

 private:
    http::AnySession* m_sess;
};
}  // namespace cent::dist
