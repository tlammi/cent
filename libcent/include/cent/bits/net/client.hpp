#pragma once

#include <cent/bits/net/concepts.hpp>
#include <cent/bits/net/result.hpp>
#include <cent/concepts.hpp>

namespace cent::net {
namespace client_detail {
template <class C>
class CtxHolder {
 protected:
    C m_ctx{};
};

template <>
class CtxHolder<std::nullptr_t> {};

template <net_concepts::session S, class C>
S make_session(C&& c) {
    if constexpr (std::is_same_v<C, std::nullptr_t>) {
        return S{};
    } else {
        return S{std::forward<C>(c)};
    }
}

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
        m_sess.on_header(std::forward<F>(f));
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

    Result get() noexcept { return m_sess.get(); }
    Result head() noexcept { return m_sess.head(); }

 private:
    S m_sess{client_detail::make_session<S>(Parent::m_ctx)};
};
}  // namespace cent::net
