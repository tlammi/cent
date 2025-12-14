#pragma once

#include <ranges>
namespace cent {

template <class T, class U>
concept range_of = std::ranges::range<T> && requires(T t) {
    { *std::ranges::begin(t) } -> std::convertible_to<U>;
};
}  // namespace cent
