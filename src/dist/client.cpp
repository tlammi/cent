#include "client.hpp"

#include <simdjson.h>

#include <ranges>

#include "cent/crypto.hpp"
#include "dist/http/smart_session.hpp"
#include "mime.hpp"
#include "util/defer.hpp"
#include "util/split.hpp"

namespace cent::dist {
namespace {

constexpr auto SCHEMA_VERSION = 2;

struct Mimed {
    std::string mediaType;
    int schemaVersion;
};

struct Platform {
    std::string architecture;
    std::string os;
};

struct ImageIndex {
    struct Entry {
        std::string digest;
        // std::string mediaType;
        Platform platform;
    };
    std::vector<Entry> manifests{};
};

struct Manifest {
    struct Ref {
        std::string digest;
        // std::string mediaType;
        size_t size;
    };

    Ref config;
    std::vector<Ref> layers;
};

void check_rfl(auto& res) {
    if (!res) raise(ErrorCode::FormatError, "{}", res.error().what());
}
void check_schema_ver(auto& mime) {
    if (mime->schemaVersion != SCHEMA_VERSION)
        raise(ErrorCode::FormatError, "invalid schema version {}",
              mime->schemaVersion);
}

std::variant<ImageIndex, Manifest> parse_manifest(std::string_view msg) {
    auto mime = rfl::json::read<Mimed>(msg);
    check_rfl(mime);
    if (mime->mediaType == mimes::oci_image_index) {
        check_schema_ver(mime);
        auto idx = rfl::json::read<ImageIndex>(msg);
        check_rfl(idx);
        return *std::move(idx);
    }
    if (mime->mediaType == mimes::oci_image_manifest) {
        check_schema_ver(mime);
        auto mfest = rfl::json::read<Manifest>(msg);
        check_rfl(mfest);
        return *std::move(mfest);
    }
    raise(ErrorCode::FormatError, "Invalid MIME: {}", mime->mediaType);
}

struct TextSink final : public http::DataSink {
    std::vector<std::vector<std::byte>> chunks{};
    bool on_status(http::StatusCode code) noexcept override {
        return code == 200;
    }
    bool on_header(std::string_view key,
                   std::string_view val) noexcept override {
        // TODO: Overriding this should be optional
        (void)key;
        (void)val;
        return true;
    }

    bool on_write(std::string_view buf) noexcept override {
        auto span = std::span<const std::byte>(
            reinterpret_cast<const std::byte*>(buf.data()), buf.size());
        chunks.emplace_back(span.begin(), span.end());
        return true;
    }
};

struct LayerSink final : public http::DataSink {
    BlobStream* stream{};
    std::exception_ptr ex{};
    bool on_status(http::StatusCode code) noexcept override {
        return code == 200;
    }
    bool on_header(std::string_view key,
                   std::string_view val) noexcept override {
        // TODO: Overriding this should be optional
        (void)key;
        (void)val;
        return true;
    }

    bool on_write(std::string_view buf) noexcept override {
        try {
            auto span = std::span<const std::byte>(
                reinterpret_cast<const std::byte*>(buf.data()), buf.size());
            stream->on_chunk(span);
        } catch (...) {
            ex = std::current_exception();
            return false;
        }
        return true;
    }
};

std::string find_manifest_ref(auto& image_index, PlatformView plat) {
    const auto& arr = image_index.manifests;
    for (const auto& entry : arr) {
        const auto& doc_plat = entry.platform;
        if (doc_plat.architecture == plat.arch && doc_plat.os == plat.os) {
            return entry.digest;
        }
    }
    raise(ErrorCode::DoesNotExist, "Could not find manifest for platform {}-{}",
          plat.arch, plat.os);
}

}  // namespace

class ClientImpl final : public Client {
 public:
    explicit ClientImpl(http::SessionPool& pool) noexcept : m_sess(pool) {}

    std::string manifest(UrlView dst) override {
        auto sink = TextSink();
        m_sess.data_sink(&sink);
        auto cleanup = util::Defer([&] { m_sess.data_sink(nullptr); });
        m_sess.set_url(dst);
        m_sess.get();
        auto range = sink.chunks | std::views::join;
        return std::string(range.begin(), range.end());
    }

    void blob(UrlView dst, BlobStream& stream) override {
        auto sink = LayerSink();
        sink.stream = &stream;
        m_sess.data_sink(&sink);
        auto cleanup = util::Defer([&] { m_sess.data_sink(nullptr); });
        m_sess.set_url(dst);
        m_sess.get();
        if (sink.ex) std::rethrow_exception(sink.ex);
    }

 private:
    http::SmartSession m_sess;
};

class DefaultClient final : public Client {
 public:
    DefaultClient() = default;

    std::string manifest(UrlView dst) override { return m_impl.manifest(dst); }
    void blob(UrlView dst, BlobStream& stream) override {
        m_impl.blob(dst, stream);
    }

 private:
    http::SimpleSessionPool m_pool;
    ClientImpl m_impl{m_pool};
};

std::unique_ptr<Client> client(http::SessionPool& session_pool) {
    return std::make_unique<ClientImpl>(session_pool);
}
std::unique_ptr<Client> client() { return std::make_unique<DefaultClient>(); }

void pull(Client& client, PullConsumer& consumer, const PullArgs& args) {
    auto url = manifest_url(args.reference);
    auto resp = client.manifest(url);
    auto manifest = parse_manifest(resp);
    if (std::holds_alternative<ImageIndex>(manifest)) {
        auto manifest_digest =
            find_manifest_ref(std::get<ImageIndex>(manifest), args.platform);
        auto manifest_nm = Name(args.reference);
        manifest_nm.set_digest(manifest_digest);
        resp = client.manifest(manifest_url(manifest_nm));
        manifest = parse_manifest(resp);
        if (!std::holds_alternative<Manifest>(manifest))
            raise(ErrorCode::FormatError, "Could not parse manifest: '{}'",
                  resp);
    }
    // TODO: Validate
    auto digest = sha256(resp);
    consumer.on_manifest(digest, resp);

    auto layers = std::get<Manifest>(manifest).layers |
                  std::views::transform([](const auto& i) -> std::string_view {
                      return i.digest;
                  }) |
                  std::ranges::to<std::vector>();

    auto config_nm = Name(args.reference);
    config_nm.set_digest(std::get<Manifest>(manifest).config.digest);

    auto cfg_blob = client.blob(blob_url(config_nm));
    auto cfg_view = std::string_view(
        reinterpret_cast<const char*>(cfg_blob.data()), cfg_blob.size());
    consumer.on_config(std::get<Manifest>(manifest).config.digest, cfg_view);
    auto layer_nm = Name(args.reference);
    for (const auto& layer : layers) {
        auto stream = consumer.layer_stream(layer);
        layer_nm.set_digest(layer);
        client.blob(blob_url(layer_nm), *stream);
    }
}
}  // namespace cent::dist
