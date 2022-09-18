#pragma once

#include <concepts>

namespace cent::concepts {

template <class T>
concept maybe_empty = requires(const T& t) {
    { t.empty() } -> std::convertible_to<bool>;
};

template <class T>
concept iterable = requires(T& t) {
    {t.begin()};
    {t.end()};
};

template <class T>
concept implicit_const_iterable = requires(const T& t) {
    {t.begin()};
    {t.end()};
};

template <class T>
concept explicit_const_iterable = requires(const T& t) {
    {t.cbegin()};
    {t.cend()};
};

}  // namespace cent::concepts