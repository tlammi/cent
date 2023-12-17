#pragma once

#include <cent/util/str_concat.hpp>
#include <iostream>
#include <stdexcept>

namespace cent {

template <class... Ts>
[[noreturn]] void raise(Ts&&... ts) {
    panic(std::forward<Ts>(ts)...);
}
template <class... Ts>
[[noreturn]] void abort(Ts&&... ts) {
    (std::cerr << ... << std::forward<Ts>(ts)) << '\n';
    ::std::abort();
}
template <class... Ts>
[[noreturn]] void panic(Ts&&... ts) {
#if defined(__cpp_exceptions)
    throw std::runtime_error(concat(std::forward<Ts>(ts)...));
#else
    ::cent::abort(std::forward<Ts>(ts)...);
#endif
}

}  // namespace cent
