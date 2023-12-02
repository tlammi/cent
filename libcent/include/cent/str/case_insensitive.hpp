#pragma once

#include <cent/str/cmp.hpp>
#include <string_view>

namespace cent {

class CaseInsensitive {
 public:
    constexpr explicit CaseInsensitive(std::string_view s) noexcept : m_s(s) {}

    constexpr auto operator<=>(std::string_view other) const noexcept {
        return threeway_compare_case_insenstive(m_s, other);
    }

 private:
    std::string_view m_s;
};
}  // namespace cent
