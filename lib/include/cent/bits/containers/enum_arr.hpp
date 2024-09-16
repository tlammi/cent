/**
 * \file
 * */
#pragma once

#include <array>
#include <cent/concepts.hpp>
#include <cent/util.hpp>
#include <magic_enum.hpp>

namespace cent {
/**
 * \brief Array where indexing is done with enumerations
 * */
template <concepts::enum_type E, class T>
class EnumArr {
 public:
    using enum_type = E;
    using value_type = T;
    constexpr EnumArr() noexcept = default;

    template <class... Ts>
    explicit constexpr EnumArr(std::in_place_type_t<T> /*unused*/, Ts&&... ts)
        : m_arr{std::forward<Ts>(ts)...} {}

    constexpr size_t size() const noexcept { return m_arr.size(); }

    template <class Self>
    constexpr auto& operator[](this Self&& self, E e) noexcept {
        const auto idx = to_index(e);
        return std::forward<Self>(self).m_arr[idx];
    }

    template <class Self>
    constexpr auto& at(this Self&& self, E e) {
        const auto idx = to_idx(e);
        return std::forward<Self>(self).m_arr.at(idx);
    }

 private:
    static constexpr size_t to_index(E e) noexcept {
        return *magic_enum::enum_index(e);
    }

    std::array<T, magic_enum::enum_count<E>()> m_arr{};
};

namespace enum_arr_detail {
template <concepts::enum_type E, class T, class F, class... Ts>
constexpr void make_enum_arr(EnumArr<E, T>& a, E e, F&& f, Ts&&... ts) {
    a[e] = std::forward<F>(f);
    if constexpr (sizeof...(Ts)) { make_enum_arr(a, std::forward<Ts>(ts)...); }
}

}  // namespace enum_arr_detail

template <concepts::enum_type E, class T, class... Ts>
constexpr auto make_enum_arr(Ts&&... ts) {
    EnumArr<E, T> arr{};
    if constexpr (sizeof...(Ts))
        enum_arr_detail::make_enum_arr(arr, std::forward<Ts>(ts)...);
    return arr;
}

}  // namespace cent

