#pragma once

#include <cent/bits/str/c_str.hpp>

namespace cent {

class StaticStr {
 public:
    constexpr StaticStr() = default;

    template <size_t S>
    consteval StaticStr(const char (&s)[S]) noexcept : m_str(s) {}
    consteval StaticStr(const char* s) noexcept : m_str(s) {}
    constexpr operator CStr() noexcept { return m_str; }

 private:
    CStr m_str{};
};
}  // namespace cent
