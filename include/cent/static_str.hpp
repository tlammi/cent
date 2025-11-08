#pragma once

#include <cent/c_str.hpp>
#include <cstddef>

namespace cent {

class StaticStr {
 public:
    template <size_t S>
    consteval StaticStr(const char (&str)[S]) : m_s(str) {}

    constexpr const char* c_str() const noexcept { return m_s.c_str(); }

 private:
    CStr m_s;
};
}  // namespace cent
