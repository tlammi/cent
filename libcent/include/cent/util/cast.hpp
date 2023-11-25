#pragma once

#include <type_traits>
namespace cent {

template <class T>
concept enum_type = std::is_enum_v<T>;

template <class T,
          class = std::enable_if_t<std::is_enum_v<std::remove_cvref_t<T>>>>
constexpr auto underlying_cast(T&& t) {
    return static_cast<std::underlying_type_t<std::remove_cvref_t<T>>>(t);
}

}  // namespace cent
