#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

namespace cent {

template <class T>
concept enum_type = std::is_enum_v<T>;

template <class T,
          class = std::enable_if_t<std::is_enum_v<std::remove_cvref_t<T>>>>
constexpr auto underlying_cast(T&& t) {
    return static_cast<std::underlying_type_t<std::remove_cvref_t<T>>>(t);
}

template <std::unsigned_integral Out, std::unsigned_integral In>
constexpr Out int_cast(In i) {
    if constexpr (sizeof(In) <= sizeof(Out)) {
        return static_cast<Out>(i);
    } else {
        In limit = std::numeric_limits<Out>::max();
        CENT_CHECK(i <= limit);
        return static_cast<Out>(i);
    }
}

template <std::signed_integral Out, std::signed_integral In>
constexpr Out int_cast(In i) {
    if constexpr (sizeof(In) <= sizeof(Out)) {
        return static_cast<Out>(i);
    } else {
        In low = std::numeric_limits<Out>::min();
        In high = std::numeric_limits<Out>::max();
        CENT_CHECK(i >= low);
        CENT_CHECK(i <= high);
        return static_cast<Out>(i);
    }
}

template <std::unsigned_integral Out, std::signed_integral In>
constexpr Out int_cast(In i) {
    CENT_CHECK(i >= 0);
    if constexpr (sizeof(In) <= sizeof(Out)) {
        return static_cast<Out>(i);
    } else {
        In limit = std::numeric_limits<Out>::max();
        CENT_CHECK(i <= limit);
        return static_cast<Out>(i);
    }
}

template <std::signed_integral Out, std::unsigned_integral In>
constexpr Out int_cast(In i) {
    if constexpr (sizeof(In) < sizeof(Out)) {
        return static_cast<Out>(i);
    } else {
        In limit = std::numeric_limits<Out>::max();
        CENT_CHECK(i <= limit);
        return static_cast<Out>(i);
    }
}

}  // namespace cent
