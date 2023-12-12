#pragma once

#include <cent/bits/net/progress.hpp>
#include <span>
#include <string_view>

namespace cent::net::net_concepts {
namespace detail {
template <class T>
struct do_mk_lambda {};

template <class R, class... Ps>
struct do_mk_lambda<R(Ps...)> {
    static constexpr auto mk() noexcept {
        return [](Ps...) -> R { return R{}; };
    }
};

template <class T>
auto mk_lambda() {
    return do_mk_lambda<T>::mk();
}
}  // namespace detail
template <class T>
concept session = requires(T t) {
    {
        t.on_header(
            detail::mk_lambda<bool(std::string_view, std::string_view)>())
    };
    { t.on_write(detail::mk_lambda<bool(std::string_view)>()) };
    { t.on_read(detail::mk_lambda<size_t(std::span<char>)>()) };
    { t.on_progress(detail::mk_lambda<bool(const Progress&)>()) };
};

}  // namespace cent::net::net_concepts
