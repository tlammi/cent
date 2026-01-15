#pragma once

#include <array>
#include <magic_enum/magic_enum.hpp>

#include "cent/concepts.hpp"

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

    template <class U>
    constexpr std::optional<Enum> index_of(const U& u) const noexcept {
        for (size_t idx = 0; idx < array_size; ++idx) {
            if (m_arr[idx] == u) return magic_enum::enum_value<Enum>(idx);
        }
        return std::nullopt;
    }

 private:
    std::array<T, array_size> m_arr{};
};

template <class T, enum_type E, class... Ts>
constexpr auto make_array(Ts&&... ts) {
    auto out = EnumArr<T, E>();
    auto apply = [&](auto&& pair) {
        out[pair.first] = std::forward<decltype(pair)>(pair).second;
    };
    (apply(std::forward<Ts>(ts)), ...);
    return out;
}

}  // namespace cent::util
