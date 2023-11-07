#pragma once

#include <tuple>

namespace cent::core::concepts {

template <class T>
concept any_iterable = requires(T t) {
    t.begin();
    t.end();
};

template <class T, class... Ts>
concept iterable = any_iterable<T> && requires(T t) {
    { *t.begin() } -> std::convertible_to<std::tuple<Ts...>>;
};
}  // namespace cent::core::concepts
