#pragma once

#include <array>
#include <cent/core/util.hpp>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace cent {

template <class E, class V, size_t S = underlying_cast(E::COUNT_)>
class EnumArr {
 public:
    template <class... Ts>
    constexpr explicit EnumArr(Ts&&... ts) : m_arr{std::forward<Ts>(ts)...} {}

    constexpr V& at(E i) { return m_arr.at(underlying_cast(i)); }
    constexpr const V& at(E i) const { return m_arr.at(underlying_cast(i)); }
    constexpr V& operator[](E i) { return at(i); }
    constexpr const V& operator[](E i) const { return at(i); }

    constexpr V& at(std::underlying_type_t<E> i) { return m_arr.at(i); }
    constexpr const V& at(std::underlying_type_t<E> i) const {
        return m_arr.at(i);
    }
    constexpr V& operator[](std::underlying_type_t<E> i) { return m_arr.at(i); }
    constexpr const V& operator[](std::underlying_type_t<E> i) const {
        return m_arr.at(i);
    }

    constexpr size_t size() const noexcept { return S; }
    constexpr size_t length() const noexcept { return S; }

    constexpr auto begin() noexcept { return m_arr.begin(); }
    constexpr auto begin() const noexcept { return m_arr.begin(); }
    constexpr auto cbegin() const noexcept { return m_arr.cbegin(); }
    constexpr auto rbegin() noexcept { return m_arr.rbegin(); }
    constexpr auto rbegin() const noexcept { return m_arr.rbegin(); }
    constexpr auto crbegin() noexcept { return m_arr.crbegin(); }
    constexpr auto crbegin() const noexcept { return m_arr.crbegin(); }

    constexpr auto end() noexcept { return m_arr.end(); }
    constexpr auto end() const noexcept { return m_arr.end(); }
    constexpr auto cend() const noexcept { return m_arr.cend(); }
    constexpr auto rend() noexcept { return m_arr.rend(); }
    constexpr auto rend() const noexcept { return m_arr.rend(); }
    constexpr auto crend() noexcept { return m_arr.crend(); }
    constexpr auto crend() const noexcept { return m_arr.crend(); }

 private:
    std::array<V, S> m_arr;
};

namespace enum_arr_detail {
template <class Arr, class Enum, class V, class... Rest>
constexpr void insert_enum_arr_recurse(Arr& a, Enum e, V&& v, Rest&&... rest) {
    static_assert(std::is_enum_v<Enum>);
    a.at(e) = std::forward<V>(v);
    if constexpr (sizeof...(Rest)) {
        insert_enum_arr_recurse(a, std::forward<Rest>(rest)...);
    }
}
}  // namespace enum_arr_detail

template <class E, class V, class... Ts>
constexpr auto make_enum_arr(Ts&&... ts) {
    EnumArr<E, V> arr{};
    enum_arr_detail::insert_enum_arr_recurse(arr, std::forward<Ts>(ts)...);
    return arr;
}
}  // namespace cent
