#pragma once

#include <variant>

namespace cent {
namespace match_detail {

template <class... Ts>
class Visitor final : Ts... {
 public:
    constexpr Visitor(Ts... ts) noexcept : Ts(ts)... {}

    using Ts::operator()...;

 private:
};
}  // namespace match_detail

template <class Var, class... Ts>
constexpr auto match(Var&& v, Ts&&... ts) {
    return std::visit(match_detail::Visitor(std::forward<Ts>(ts)...),
                      std::forward<Var>(v));
}

}  // namespace cent
