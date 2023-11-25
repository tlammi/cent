#pragma once

#include <string_view>

namespace cent {

class StaticStr {
 public:
    consteval StaticStr(const char* s) : m_s(s) {}
    constexpr operator const char*() const noexcept { return m_s; }
    constexpr std::string_view view() const noexcept { return {m_s}; }
    constexpr const char* c_str() const noexcept { return m_s; }

 private:
    const char* m_s;
};
}  // namespace cent
