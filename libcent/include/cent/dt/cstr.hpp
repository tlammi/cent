#pragma once

#include <string_view>

namespace cent {

class CStr {
 public:
    explicit constexpr CStr(const char* s) : m_s{s} {}
    constexpr const char* c_str() const noexcept { return m_s; }
    constexpr std::string_view view() const noexcept { return {m_s}; }

    constexpr bool operator==(const char* s) const noexcept {
        return (*this <=> s) == 0;
    }

    constexpr int operator<=>(const char* s) const noexcept {
        size_t i = 0;
        while (s[i] && m_s[i]) {
            if (m_s[i] < s[i]) return -1;
            if (m_s[i] > s[i]) return 1;
            ++i;
        }
        if (!m_s[i]) return -1;
        if (!s[i]) return 1;
        return 0;
    }

 private:
    const char* m_s;
};
}  // namespace cent
