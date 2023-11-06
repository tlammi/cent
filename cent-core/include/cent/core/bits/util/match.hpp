#pragma once

#include <variant>

namespace cent::core {
namespace match_detail {

template <class... Ts>
struct Visitor final : Ts... {
    template <class... Ts2>
    Visitor(Ts2&&... ts) : Ts{std::forward<Ts2>(ts)}... {}
    using Ts::operator()...;
};

template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

}  // namespace match_detail

template <class T, class... Ts>
constexpr auto match(T&& val, Ts&&... ts) {
    return std::visit(match_detail::Visitor{std::forward<Ts>(ts)...},
                      std::forward<T>(val));
}
}  // namespace cent
