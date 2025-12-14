#include "smart_session.hpp"

#include <print>
#include <rfl/json.hpp>

#include "error.hpp"
#include "util/case_insensitive.hpp"
#include "util/split.hpp"
#include "util/strip.hpp"

namespace cent::dist::http {
using namespace std::literals;
namespace {

// HDR< www-authenticate:  Bearer
// realm="https://auth.docker.io/token",service="registry.docker.io",scope="repository:library/alpine:pull"
struct ChallengeData {
    std::string_view realm;
    std::string_view service;
    std::string_view scope;
};

constexpr const std::string& find_www_auth(const auto& hdrs) {
    auto it = std::ranges::find_if(hdrs, [&](const auto& pair) {
        return util::CaseInsensitive(pair.first) == "www-authenticate";
    });
    if (it == hdrs.end())
        raise(ErrorCode::Internal, "www-auth header disappeared");
    return it->second;
}

constexpr ChallengeData parse_challenge(
    const std::vector<std::pair<std::string, std::string>>& hdrs) {
    ChallengeData out{};
    const auto& v = find_www_auth(hdrs);
    for (auto item : v | std::views::split(',')) {
        auto [key, val] = util::split_first(std::string_view(item), "="sv);
        key = util::strip(key);
        val = util::strip(val);
        val = util::strip(val, '"');
        if (util::CaseInsensitive(key) == "bearer realm") {
            out.realm = val;
        } else if (util::CaseInsensitive(key) == "service") {
            out.service = val;
        } else if (util::CaseInsensitive(key) == "scope") {
            out.scope = val;
        }
    }
    return out;
}

enum class State {
    Buffering,
    PassThrough,
    Challenge,
};

struct PrimaryDataSink final : public DataSink {
    StatusCode active_code{};
    State state{};
    DataSink* child_sink{};
    std::vector<std::pair<std::string, std::string>> header_buffer{};

    bool on_status(StatusCode code) noexcept override {
        active_code = code;
        if (code == 401) {
            state = State::Buffering;
            return true;
        }
        if (code == 307) { return true; }
        state = State::PassThrough;
        return child_sink->on_status(code);
    }

    bool on_header(std::string_view key,
                   std::string_view val) noexcept override {
        using enum State;
        switch (state) {
            case Buffering:
                if (util::CaseInsensitive(key) == "www-authenticate") {
                    state = Challenge;
                }
                [[fallthrough]];
            case Challenge:
                header_buffer.emplace_back(std::string(key), std::string(val));
                return true;
            case PassThrough: return child_sink->on_header(key, val);
        }
        std::unreachable();
    }

    bool on_write(std::string_view buf) noexcept override {
        using enum State;
        switch (state) {
            case Buffering:
                // Received 401 but no auth challenge -> forward headers to
                // child
                if (!child_sink->on_status(active_code)) return false;
                for (const auto& [key, val] : header_buffer) {
                    if (!child_sink->on_header(key, val)) return false;
                }
                state = PassThrough;
                [[fallthrough]];
            case PassThrough: return child_sink->on_write(buf);
            case Challenge: return true;
        }
        std::unreachable();
    }
};
}  // namespace

struct SmartSession::Impl {
    PrimaryDataSink data_sink{};
    SessionPool* pool{};
    SessionPool::Ptr primary{pool->session()};
};

SmartSession::SmartSession(SessionPool& sess_pool)
    : m_impl(new Impl{.pool = &sess_pool}) {}

SmartSession::~SmartSession() = default;

void SmartSession::data_sink(DataSink* sink) {
    m_impl->data_sink.child_sink = sink;
    if (sink)
        m_impl->primary->data_sink(&m_impl->data_sink);
    else
        m_impl->primary->data_sink(nullptr);
}

void SmartSession::data_src(DataSrc* src) { m_impl->primary->data_src(src); }

void SmartSession::set_header(std::string_view key, std::string_view val) {
    m_impl->primary->set_header(key, val);
}

void SmartSession::set_url(const Url& url) { m_impl->primary->set_url(url); }

void SmartSession::get() {
    m_impl->primary->get();
    if (m_impl->data_sink.state == State::Challenge) {
        auto challenge = parse_challenge(m_impl->data_sink.header_buffer);
        auto secondary = m_impl->pool->session();
        auto url = Url(std::format("{}?service={}&scope={}", challenge.realm,
                                   challenge.service, challenge.scope));
        secondary->set_url(url);
        struct Sink final : DataSink {
            std::string buffer{};
            bool on_status(StatusCode /*code*/) noexcept override {
                return true;
            }
            bool on_header(std::string_view /*key*/,
                           std::string_view /*val*/) noexcept override {
                return true;
            }
            bool on_write(std::string_view data) noexcept override {
                // TODO: catch and gracefully handle
                buffer.append(data);
                return true;
            }
        };
        auto sink = Sink();
        secondary->data_sink(&sink);
        secondary->get();
        struct WithToken {
            std::string token;
        };
        auto res = rfl::json::read<WithToken>(sink.buffer);
        if (!res) raise(ErrorCode::Generic, "{}", res.error().what());
        m_impl->primary->set_header("Authorization",
                                    std::format("Bearer {}", res->token));
        m_impl->primary->get();
    }
}

}  // namespace cent::dist::http
