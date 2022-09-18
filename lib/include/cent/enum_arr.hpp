/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <array>
#include <cstddef>
#include <optional>

#include "cent/type_traits.hpp"

namespace cent {

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