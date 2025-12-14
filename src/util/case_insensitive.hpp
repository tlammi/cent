#pragma once

#include <ranges>
#include <string_view>

namespace cent::util {

class CaseInsensitive {
 public:
    explicit constexpr CaseInsensitive(std::string_view s) noexcept : m_s(s) {}

    constexpr bool operator==(std::string_view str) const noexcept {
        static constexpr auto to_lower = [](char c) {
            if (c >= 'A' && c <= 'Z')
                return static_cast<char>(static_cast<char>(c - 'A') + 'a');
            return c;
        };
        for (const auto& [l, r] : std::views::zip(m_s, str)) {
            if (to_lower(l) != to_lower(r)) return false;
        }
        return true;
    }

 private:
    std::string_view m_s{};
};
}  // namespace cent::util
