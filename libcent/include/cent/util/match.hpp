#pragma once

#include <utility>
#include <variant>

namespace cent {
namespace match_detail {
template <class... Ts>
struct Visitor final : public Ts... {
    Visitor(Ts&&... ts) : Ts(std::forward<Ts>(ts))... {}
    using Ts::operator()...;
};
template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

}  // namespace match_detail

template <class Var, class... Ts>
constexpr auto match(Var&& v, Ts&&... ts) {
    match_detail::Visitor visit(std::forward<Ts>(ts)...);
    return std::visit(visit, std::forward<Var>(v));
}
}  // namespace cent
