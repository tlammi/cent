#pragma once

#include <memory>

#include "dist/http/session_pool.hpp"
#include "dist/url.hpp"
#include "img_config.hpp"
#include "manifest.hpp"
#include "platform.hpp"

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

    [[nodiscard]] std::vector<std::byte> blob(UrlView dst) {
        struct Stream final : BlobStream {
            std::vector<std::byte> res{};
            void on_chunk(std::span<const std::byte> data) override {
                res.append_range(data);
            }
        };
        Stream s{};
        blob(dst, s);
        return std::move(s).res;
    }
};

std::unique_ptr<Client> client(http::SessionPool& session_pool);
std::unique_ptr<Client> client();

/**
 * \brief Callbacks invoked when pulling an image
 *
 * The methods are called in order manifest -> config -> layers...
 * */
class PullConsumer {
    struct Deleter {
        PullConsumer* consumer;
        void operator()(BlobStream* stream) {
            consumer->free_layer_stream(stream);
        }
    };

 public:
    using BlobStreamPtr = std::unique_ptr<BlobStream, Deleter>;

    /**
     * \brief Consume the received manifest
     * */
    virtual void on_manifest(std::string_view digest,
                             std::string_view mfest) = 0;
    /**
     * \brief Consume the received image config
     * */
    virtual void on_config(std::string_view digest, std::string_view cfg) = 0;

    template <class... Ts>
    BlobStreamPtr layer_stream(Ts&&... ts) {
        return BlobStreamPtr{get_layer_stream(std::forward<Ts>(ts)...),
                             Deleter{this}};
    }

 protected:
    ~PullConsumer() = default;

 private:
    /**
     * \brief Consume one of the image layers
     * */
    virtual BlobStream* get_layer_stream(std::string_view digest) = 0;
    virtual void free_layer_stream(BlobStream* stream) = 0;
};

struct PullArgs {
    NameView reference;
    PlatformView platform = DEFAULT_PLATFORM;
};

void pull(Client& client, PullConsumer& consumer, const PullArgs& args);

}  // namespace cent::dist
