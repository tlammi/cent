/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <sstream>
#include <stdexcept>

namespace cent {
namespace detail {

template <class T, class... Ts>
void accumulate_ss(std::stringstream& ss, T&& t, Ts&&... ts) {
    ss << std::forward<T>(t);
    if constexpr (sizeof...(Ts)) { accumulate_ss(ss, std::forward<Ts>(ts)...); }
}
}  // namespace detail

template <class... Ts>
[[noreturn]] void raise(Ts&&... ts) {
    std::stringstream ss;
    detail::accumulate_ss(ss, std::forward<Ts>(ts)...);
    throw std::runtime_error(ss.str());
}
}  // namespace cent