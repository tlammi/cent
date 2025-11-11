#pragma once

#include <array>
#include <cent/util/concepts.hpp>
#include <magic_enum.hpp>

namespace cent::util {

template <class T, enum_type Enum>
class EnumArr {
 public:
    constexpr EnumArr() = default;
    template <class... Ts>
    constexpr explicit EnumArr(std::in_place_t /*tag*/, Ts&&... ts)
        : m_arr{std::forward<Ts>(ts)...} {}

    static constexpr auto array_size = magic_enum::enum_count<Enum>();

    constexpr auto size() const noexcept { return array_size; }

    template <class S>
    constexpr decltype(auto) operator[](this S&& s, Enum idx) noexcept {
        return std::forward<S>(s).m_arr[*magic_enum::enum_index(idx)];
    }

 private:
    std::array<T, array_size> m_arr{};
};
}  // namespace cent::util
