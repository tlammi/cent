/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <limits>
#include <utility>

#include "cent/raise.hpp"

namespace cent::util {

/**
 * Safe cast integer to another
 *
 * \param i Input
 * \return Casted value
 */
template <class O, class I>
    requires(std::integral<O> && std::integral<I>)
constexpr O int_cast(I i) {
    auto out_max = std::numeric_limits<O>::max();
    auto out_min = std::numeric_limits<O>::min();

    if (std::cmp_greater(i, out_max)) raise("Tool large to cast: ", i);
    if (std::cmp_less(i, out_min)) raise("Too small to cast: ", i);
    return static_cast<O>(i);
}

/**
 * Cast an enum to its underlying type
 */
template <class Enum>
constexpr auto underlying_cast(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

/**
 * Create an enum from an underlying type
 */
template <class Enum>
constexpr Enum underlying_cast(std::underlying_type_t<Enum> v) {
    return static_cast<Enum>(v);
}
}  // namespace cent::util