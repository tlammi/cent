#pragma once

#include <cent/c_str.hpp>
#include <cstddef>

namespace cent {

class StaticStr {
 public:
    template <size_t S>
    consteval StaticStr(const char (&str)[S]) : m_s(str, S - 1) {}

    constexpr const char* c_str() const noexcept { return m_s.data(); }
    constexpr auto string_view() const noexcept { return m_s; }

 private:
    std::string_view m_s;
};
}  // namespace cent
