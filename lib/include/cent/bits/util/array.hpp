#pragma once

#include <array>
#include <vector>

namespace cent {

template <class T, class... Ts>
constexpr auto make_array(Ts&&... ts) {
    return std::array<T, sizeof...(Ts)>(std::forward<Ts>(ts)...);
}

template <class T, class... Ts>
constexpr auto make_vector(Ts&&... ts) {
    std::vector<T> out{};
    out.reserve(sizeof...(Ts));
    (out.emplace_back(std::forward<Ts>(ts)), ...);
    return out;
}
}  // namespace cent
