#pragma once

#include <cent/data/image_index.hpp>
#include <cent/data/manifest.hpp>
#include <cent/dist/http/session.hpp>

namespace cent::dist {

class LayerStream {
 public:
    virtual void on_chunk(std::span<const std::byte> data) = 0;

 protected:
    ~LayerStream() = default;
};

class RegistryClient {
 public:
    explicit RegistryClient(http::AnySession& sess) noexcept;

    std::variant<data::ImageIdx, data::Manifest> manifest(UrlView url);

    void layer(UrlView url, LayerStream& stream);

 private:
    http::AnySession* m_sess;
};
}  // namespace cent::dist
