#pragma once

#include <cent/str/cmp.hpp>
#include <cent/util.hpp>
#include <string_view>

namespace cent {

class CaseInsensitive {
 public:
    constexpr explicit CaseInsensitive(std::string_view s) noexcept : m_s(s) {}

    constexpr bool operator==(std::string_view other) const noexcept {
        return m_s <=> other == std::strong_ordering::equal;
    }
    constexpr auto operator<=>(std::string_view other) const noexcept {
        return threeway_compare_case_insenstive(m_s, other);
    }

    constexpr auto find(std::string_view tok) const noexcept {
        auto iter = std::search(
            m_s.begin(), m_s.end(), tok.begin(), tok.end(),
            [](char l, char r) { return to_lower(l) == to_lower(r); });
        if (iter == m_s.end()) return std::string_view::npos;
        return int_cast<size_t>(iter - m_s.begin());
    }

    constexpr auto find(char c) const noexcept {
        auto iter = std::find_if(m_s.begin(), m_s.end(), [&](char c) {
            return to_lower(c) == to_lower(c);
        });
        if (iter == m_s.end()) return std::string_view::npos;
        return int_cast<size_t>(iter - m_s.begin());
    }

    void remove_prefix(size_t count) { m_s.remove_prefix(count); }

 private:
    std::string_view m_s;
};
}  // namespace cent
