#include <cent/dist/client.hpp>
#include <cent/dist/http/smart_session.hpp>
#include <cent/util/defer.hpp>
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

}  // namespace cent::dist
