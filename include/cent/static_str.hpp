#pragma once

#include <cent/c_str.hpp>
#include <cstddef>

namespace cent {

class StaticStr {
 public:
    constexpr StaticStr() noexcept = default;

    consteval StaticStr(const char* str) : m_s(str) {}

    template <size_t S>
    consteval StaticStr(const char (&str)[S]) : m_s(str, S - 1) {}

    constexpr const char* c_str() const noexcept { return m_s.data(); }
    constexpr auto string_view() const noexcept { return m_s; }

    constexpr auto operator<=>(const StaticStr&) const noexcept = default;

    constexpr bool operator==(std::string_view v) const noexcept {
        return m_s == v;
    }

 private:
    std::string_view m_s;
};
}  // namespace cent
