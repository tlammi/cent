#include <cent/dist/registry_client.hpp>
#include <cent/util/defer.hpp>
#include <print>
#include <ranges>

namespace cent::dist {

namespace {

struct DataSink final : public http::DataSink {
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

}  // namespace

RegistryClient::RegistryClient(http::AnySession& sess) noexcept
    : m_sess(&sess) {}

data::ImageIdx RegistryClient::manifest(http::UrlView url) {
    auto sink = DataSink();
    m_sess->data_sink(&sink);
    auto cleanup = util::Defer([&] { m_sess->data_sink(nullptr); });
    m_sess->set_url(url);
    m_sess->get();
    auto range = sink.chunks | std::views::join;
    return data::parse_manifest(std::string(range.begin(), range.end()));
}

}  // namespace cent::dist
