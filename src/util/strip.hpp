#pragma once

#include <string_view>

namespace cent::util {

constexpr bool is_space(char c) noexcept {
    // TODO: Check other cases but these work with URls
    switch (c) {
        case ' ': return true;
        case '\n': return true;
        case '\r': return true;
        default: return false;
    }
}

constexpr std::string_view strip_front(std::string_view s) noexcept {
    while (!s.empty() && is_space(s.front())) s.remove_prefix(1);
    return s;
}

constexpr std::string_view strip_front(std::string_view s, auto tok) noexcept {
    while (!s.empty() && s.front() == tok) s.remove_prefix(1);
    return s;
}

constexpr std::string_view strip_back(std::string_view s) noexcept {
    while (!s.empty() && is_space(s.back())) s.remove_suffix(1);
    return s;
}

constexpr std::string_view strip_back(std::string_view s, auto tok) noexcept {
    while (!s.empty() && s.back() == tok) s.remove_suffix(1);
    return s;
}

constexpr std::string_view strip(std::string_view s) noexcept {
    return strip_back(strip_front(s));
}

constexpr std::string_view strip(std::string_view s, auto tok) noexcept {
    return strip_back(strip_front(s, tok), tok);
}

}  // namespace cent::util
