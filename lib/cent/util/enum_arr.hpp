/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <array>
#include <cstddef>
#include <optional>

#include "cent/util/cast.hpp"

namespace cent::util {

/**
 * Array with enums as indexes
 *
 * This type works with enumerations which get values from 0 upwards.
 * The enumeration's max value can be set manually or automatically deduced from
 * COUNT_ value.
 *
 * Example:
 * ```
 * struct Key{
 *      A,
 *      B,
 *      COUNT_,
 * };
 *
 * EnumArr<Key, std::string> map{};
 * map[Key::A] = "A";
 * map[Key::B] = "B";
 * ```
 * \tparam Enum Enumeration to use as key
 * \tparam V Value type to store
 * \tparam Array size. Deduced automatically from \a Enum::COUNT_ if available
 */
template <class Enum, class V, size_t Size = underlying_cast(Enum::COUNT_)>
class EnumArr {
 public:
    template <class... Ts>
    constexpr EnumArr(Ts&&... ts) : m_arr{std::forward<Ts>(ts)...} {
        static_assert(sizeof...(Ts) <= Size);
    }

    constexpr V& operator[](Enum e) { return m_arr.at(underlying_cast(e)); }
    constexpr const V& operator[](Enum e) const {
        return m_arr.at(underlying_cast(e));
    }

    constexpr std::optional<Enum> find(const V& v) const noexcept {
        for (size_t i = 0; i < size(); ++i) {
            if (m_arr.at(i) == v) return static_cast<Enum>(i);
        }
        return std::nullopt;
    }

    constexpr size_t size() const noexcept { return Size; }

 private:
    std::array<V, Size> m_arr{};
};
}  // namespace cent