#include <cent/dist/registry_client.hpp>
#include <cent/util/defer.hpp>
#include <print>
#include <ranges>

namespace cent::dist {

namespace {

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
    LayerStream* stream{};
    std::exception_ptr ex{};
    bool on_status(http::StatusCode code) noexcept override {
        return code == 200 || code == 307;
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

}  // namespace

RegistryClient::RegistryClient(http::AnySession& sess) noexcept
    : m_sess(&sess) {}

std::variant<data::ImageIdx, data::Manifest> RegistryClient::manifest(
    UrlView url) {
    auto sink = TextSink();
    m_sess->data_sink(&sink);
    auto cleanup = util::Defer([&] { m_sess->data_sink(nullptr); });
    m_sess->set_url(url);
    m_sess->get();
    auto range = sink.chunks | std::views::join;
    auto str = std::string(range.begin(), range.end());
    auto res = data::parse_image_index(str);
    if (!res) { return data::parse_manifest(str).unwrap(); }
    return data::parse_image_index(std::string(range.begin(), range.end()))
        .unwrap();
}
void RegistryClient::layer(UrlView url, LayerStream& stream) {
    auto sink = LayerSink();
    sink.stream = &stream;
    m_sess->data_sink(&sink);
    auto cleanup = util::Defer([&] { m_sess->data_sink(nullptr); });
    m_sess->set_url(url);
    m_sess->get();
}

}  // namespace cent::dist
