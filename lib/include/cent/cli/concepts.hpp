#pragma once

#include <string>
#include <type_traits>
#include <vector>

namespace cent::cli::concepts {
namespace detail {
template <class T>
struct is_vector_impl : std::false_type {};
template <class T, class A>
struct is_vector_impl<std::vector<T, A>> : std::true_type{};

}  // namespace detail

template <class T>
concept vector = detail::is_vector_impl<T>::value;

template <class T>
concept string = std::is_same_v<T, std::string>;

}  // namespace cent::cli::concepts
