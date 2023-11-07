#pragma once

#include <array>

namespace cent::core {

template <class T, class... Ts>
constexpr std::array<T, sizeof...(Ts)> make_array(Ts&&... ts) {
    return {std::forward<Ts>(ts)...};
}
}  // namespace cent::core
