#pragma once

#include <cent/util/str_concat.hpp>
#include <stdexcept>

namespace cent {

template <class... Ts>
[[noreturn]] void raise(Ts&&... ts) {
#if defined(__cpp_exceptions)
    throw std::runtime_error(concat(std::forward<Ts>(ts)...));
#else
    std::abort();
#endif
}

}  // namespace cent
