#pragma once

#include <limits>
#include <utility>

#include "cent/raise.hpp"

namespace cent {

template <class O, class I>
requires(std::integral<O>&& std::integral<I>) constexpr O int_cast(I i) {
    auto out_max = std::numeric_limits<O>::max();
    auto out_min = std::numeric_limits<O>::min();

    if (std::cmp_greater(i, out_max)) raise("Tool large to cast: ", i);
    if (std::cmp_less(i, out_min)) raise("Too small to cast: ", i);
    return static_cast<O>(i);
}
}  // namespace cent