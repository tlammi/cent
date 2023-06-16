#pragma once

#include <cstddef>
#include <string_view>
namespace cent {

class StaticStr {
 public:
    template <size_t S>
    consteval StaticStr(const char (&str)[S])
        : m_s{str}, m_size{S} {}  // NOLINT

    consteval StaticStr(const char* str)
        : m_s{str}, m_size{std::string_view(str).size()} {}

    constexpr std::string_view view() const noexcept { return {m_s, m_size}; }
    constexpr operator std::string_view() const noexcept { return view(); }

    constexpr const char* c_str() const noexcept { return m_s; }
    constexpr const char* data() const noexcept { return m_s; }
    constexpr operator const char*() const noexcept { return m_s; }

 private:
    const char* m_s;
    size_t m_size;
};

namespace literals {
consteval StaticStr operator""_ss(const char* str) { return StaticStr{str}; }
}  // namespace literals
}  // namespace cent
