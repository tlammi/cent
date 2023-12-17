#pragma once

#include <fmt/format.h>

#include <cent/bits/net/concepts.hpp>
#include <cent/bits/net/result.hpp>
#include <cent/concepts.hpp>
#include <cent/str.hpp>

namespace cent::net {
namespace client_detail {

template <class C>
class CtxHolder {
 protected:
    template <net_concepts::session S>
    auto mk_session() {
        return S{m_ctx};
    }

 private:
    C m_ctx{};
};

template <>
class CtxHolder<std::nullptr_t> {
 protected:
    template <net_concepts::session S>
    auto mk_session() {
        return S{};
    }
};

}  // namespace client_detail

/**
 * \brief "Smart session"
 *
 * This object wraps a session and resolves http authentication possibly using
 * other sessions.
 * */
template <net_concepts::session S, class C = std::nullptr_t>
class Client final : private client_detail::CtxHolder<C> {
    using Parent = client_detail::CtxHolder<C>;

 public:
    using session_type = S;

    template <concepts::proto<bool(std::string_view, std::string_view)> F>
    void on_header(F&& f) noexcept {
        m_sess.on_header(decorate_on_header(std::forward<F>(f)));
    }

    template <concepts::proto<bool(std::string_view)> F>
    void on_write(F&& f) noexcept {
        m_sess.on_write(std::forward<F>(f));
    }

    template <concepts::proto<bool(std::span<char>)> F>
    void on_read(F&& f) noexcept {
        m_sess.on_read(std::forward<F>(f));
    }

    template <concepts::proto<bool(const Progress&)> F>
    void on_progress(F&& f) noexcept {
        m_sess.on_progress(std::forward<F>(f));
    }

    void url(const char* url) noexcept { m_sess.url(url); }
    void url(const std::string& url) noexcept { m_sess.url(url.c_str()); }

    Result get() noexcept {
        auto res = m_sess.get();
        if (!res) {
            if (m_www_auth.empty()) return res;
            resolve_www_auth();
            return res;
        }
        return res;
    }
    Result head() noexcept { return m_sess.head(); }

 private:
    template <class F>
    auto decorate_on_header(F&& f) noexcept {
        return [&, f = std::forward<F>(f)](std::string_view key,
                                           std::string_view val) -> bool {
            if (key == "www-authenticate") {
                m_www_auth = std::string(val);
                std::cerr << "www auth: " << m_www_auth << '\n';
                return false;
            }
            return f(key, val);
        };
    }

    void resolve_www_auth() {
        S auth_sess{};
        std::string bearer_realm{};
        std::string service{};
        std::string scope{};

        for (auto [rkey, val] : StrMap(m_www_auth)) {
            auto key = CaseInsensitive(rkey);
            if (key == "bearer realm") {
                bearer_realm = strip(val, '"');
            } else if (key == "service") {
                service = strip(val, '"');
            } else if (key == "scope") {
                scope = strip(val, '"');
            } else {
                panic("Unsupported WWW-Auth header: '", rkey, "'\n");
            }
        }
        auto url =
            fmt::format("{}?service={}&scope={}", bearer_realm, service, scope);
        std::cerr << "challenge url: " << url << '\n';
        auth_sess.on_header(
            [&](std::string_view key, std::string_view val) -> bool {
                std::cerr << "challenge header: " << key << ' ' << val << '\n';
                return true;
            });
        auth_sess.on_write([&](std::string_view buf) -> bool {
            std::cerr << "challenge body: " << buf << '\n';
            return true;
        });
        auth_sess.url(url.c_str());
        auto res = auth_sess.get();
        (void)res;
    }

    std::string m_www_auth{};
    S m_sess{this->template mk_session<S>()};
};
}  // namespace cent::net
