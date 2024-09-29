#pragma once

#include <type_traits>

namespace cent::concepts {
namespace detail {
template <class T>
concept pointerlike_impl = requires(T t) {
    { t.operator*() };
    { t.operator->() };
};
}  // namespace detail
template <class E>
concept enum_type = std::is_enum_v<E>;

template <class T>
concept pointerlike = detail::pointerlike_impl<T> || std::is_pointer_v<T>;

}  // namespace cent::concepts
