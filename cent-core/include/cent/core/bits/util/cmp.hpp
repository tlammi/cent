#pragma once

#include <iostream>
#include <type_traits>

namespace cent::core {

namespace cmp_detail {

template <class T>
struct is_ref_wrapper : std::false_type {};

template <class T>
struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <class T>
constexpr bool is_ref_wrapper_v = is_ref_wrapper<std::remove_cvref_t<T>>::value;

template <class T>
const auto& get(const T& t) {
    if constexpr (is_ref_wrapper_v<T>) {
        return t.get();
    } else {
        return t;
    }
}
}  // namespace cmp_detail

template <class F>
struct Cmp {
    F func;
    template <class T>
    constexpr bool operator==(const T& v) const noexcept {
        return func(v);
    }
};

template <class F>
Cmp(F) -> Cmp<F>;

template <class C, class... Ts>
constexpr auto any_of_if(C&& c, Ts&&... ts) {
    return Cmp{
        [c = std::forward<C>(c), ... ts = std::forward<Ts>(ts)](const auto& v) {
            return (c(v, cmp_detail::get(ts)) || ...);
        }};
}

template <class... Ts>
constexpr auto any_of(Ts&&... ts) {
    return any_of_if([](const auto& l, const auto& r) { return l == r; },
                     std::forward<Ts>(ts)...);
}

template <class C, class... Ts>
constexpr auto none_of_if(C&& c, Ts&&... ts) {
    return Cmp{
        [c = std::forward<C>(c), ... ts = std::forward<Ts>(ts)](const auto& v) {
            return !(c(v, cmp_detail::get(ts)) || ...);
        }};
}

template <class... Ts>
constexpr auto none_of(Ts&&... ts) {
    return none_of_if([](const auto& l, const auto& r) { return l == r; },
                      std::forward<Ts>(ts)...);
}

template <class C, class... Ts>
constexpr auto all_of_if(C&& c, Ts&&... ts) {
    return Cmp{
        [c = std::forward<C>(c), ... ts = std::forward<Ts>(ts)](const auto& v) {
            return (c(v, cmp_detail::get(ts)) && ...);
        }};
}

template <class... Ts>
constexpr auto all_of(Ts&&... ts) {
    return all_of_if([](const auto& l, const auto& r) { return l == r; },
                     std::forward<Ts>(ts)...);
}

}  // namespace cent
