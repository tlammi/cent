#pragma once

#include <cstddef>
#include <string_view>

namespace cent {

template <class CharT>
constexpr size_t str_len(const CharT* t) noexcept {
    return std::basic_string_view<CharT>(t).size();
}

template <class CharT>
constexpr CharT* str_dup(std::basic_string_view<CharT> s) {
    auto* p = new CharT[s.size() + 1];
    p[s.size()] = '\0';
    for (size_t i = 0; i < s.size(); ++i) { p[i] = s[i]; }
    return p;
}
}  // namespace cent
