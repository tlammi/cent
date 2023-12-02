#pragma once

#include <concepts>
#include <type_traits>

namespace cent::concepts {

namespace detail {

template <class T, class P>
struct proto_impl;

template <class T, class R, class... Ps>
struct proto_impl<T, R(Ps...)> {
    static constexpr bool value =
        std::convertible_to<std::invoke_result_t<T, Ps...>, R>;
};

}  // namespace detail
template <class T>
concept enum_type = std::is_enum_v<T>;

template <class T, class P>
concept proto = detail::proto_impl<T, P>::value;

template <class T>
concept move_assignable = std::assignable_from<T&, T>;

template <class T>
concept copy_assignable =
    std::assignable_from<T&, T&> && std::assignable_from<T&, const T&> &&
    std::assignable_from<T&, const T>;

}  // namespace cent::concepts
