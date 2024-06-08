#pragma once

#include <exception>

namespace cent {
[[noreturn]] constexpr void panic() {
#if defined(__cpp_exceptions)
    throw std::runtime_error("");
#else
    std::terminate();
#endif
}

}  // namespace cent
