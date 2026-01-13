#pragma once

#include <ranges>
namespace cent {

template <class T, class U>
concept range_of = std::ranges::range<T> && requires(T t) {
    { *std::ranges::begin(t) } -> std::convertible_to<U>;
};

template <class T>
concept character_type = std::integral<T> && sizeof(T) == 1;

template <class T>
concept enum_type = std::is_enum_v<T>;

template <class T>
concept anyspanlike = requires(T t) {
    { t.data() };
    { t.size() } -> std::integral;
};
template <class T, class U>
concept spanlike = anyspanlike<T> && requires(T t) {
    { t.data() } -> std::convertible_to<U*>;
};
}  // namespace cent
