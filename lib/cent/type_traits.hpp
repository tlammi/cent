/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <type_traits>

namespace cent {

template <class Enum>
constexpr auto underlying_cast(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class Enum>
constexpr Enum underlying_cast(std::underlying_type_t<Enum> v) {
    return static_cast<Enum>(v);
}

}  // namespace cent