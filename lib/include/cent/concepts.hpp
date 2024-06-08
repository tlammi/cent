#pragma once

#include <concepts>
#include <type_traits>

namespace cent::concepts {
namespace detail {

struct MockCallable {
    constexpr void operator()() const noexcept {}
};

template <class T>
struct Wrap : MockCallable, T {};

template <class T, class = void>
struct has_call_operator_impl : std::true_type {};

template <class T>
struct has_call_operator_impl<T, std::void_t<decltype(&T::operator())>>
    : std::false_type{};

template <class T>
constexpr bool has_call_operator_v = has_call_operator_impl<Wrap<T>>::value;

template <class T>
struct is_function_type : std::false_type {};

template <class R, class... Ps>
struct is_function_type<R(Ps...)> : std::true_type{};

template <class T>
constexpr bool is_function_type_v = is_function_type<T>::value;

}  // namespace detail

/**
 * \brief Concept for any callable
 *
 * True for functions, function pointers and functors.
 * */
template <class T>
concept any_callable = detail::is_function_type_v<std::remove_pointer_t<T>> ||
                       (std::is_class_v<T> && detail::has_call_operator_v<T>);

template <class T>
concept boolean = std::same_as<T, bool>;

}  // namespace cent::concepts
