#pragma once

#include <cent/dist/http/session_pool.hpp>
#include <cent/dist/url.hpp>
#include <memory>

namespace cent::dist {

class BlobStream {
 public:
    virtual void on_chunk(std::span<const std::byte> data) = 0;

 protected:
    ~BlobStream() = default;
};

class Client {
 public:
    virtual ~Client() = default;

    virtual std::string manifest(UrlView dst) = 0;
    virtual void blob(UrlView dst, BlobStream& stream) = 0;
};

std::unique_ptr<Client> client(http::SessionPool& session_pool);
std::unique_ptr<Client> client();

}  // namespace cent::dist
