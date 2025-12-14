#pragma once

#include "concepts.hpp"

namespace cent {
template <enum_type E>
constexpr auto underlying_cast(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

template <enum_type E>
constexpr auto underlying_cast(std::underlying_type_t<E> t) noexcept {
    return static_cast<E>(t);
}
}  // namespace cent
