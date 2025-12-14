#pragma once

#include <array>

namespace cent::util {

template <class T, class... Ts>
constexpr auto make_array(Ts&&... ts) {
    return std::array<T, sizeof...(Ts)>{std::forward<Ts>(ts)...};
}
}  // namespace cent::util
