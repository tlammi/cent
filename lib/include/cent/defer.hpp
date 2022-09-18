/* SPDX-License-Identifier:  GPL-3.0-or-later */

#pragma once

#include <concepts>
#include <utility>

namespace cent {

template <class F = std::nullptr_t>
requires(std::invocable<F> || std::same_as<F, std::nullptr_t>) class Defer {
 public:
    template <class F2>
    Defer(F2&& f) : m_f{std::forward<F>(f)} {}

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;

    Defer(Defer&&) = delete;
    Defer& operator=(Defer&&) = delete;

    ~Defer() { m_f(); }

 private:
    F m_f;
};

/// No-op defer
template <>
class Defer<std::nullptr_t> {};

template <class T>
Defer(T&&) -> Defer<T>;
Defer()->Defer<std::nullptr_t>;

}  // namespace cent