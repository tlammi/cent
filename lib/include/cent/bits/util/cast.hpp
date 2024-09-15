/**
 * \brief Casting utilities
 * */
#pragma once

#include <cent/concepts.hpp>
#include <type_traits>

#include "cent/bits/concepts.hpp"

namespace cent {

template <concepts::enum_type E>
constexpr std::underlying_type_t<E> underlying_cast(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

template <concepts::enum_type E>
constexpr E underlying_cast(std::underlying_type_t<E> t) noexcept {
    return static_cast<E>(t);
}

}  // namespace cent
