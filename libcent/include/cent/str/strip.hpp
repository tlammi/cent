#pragma once

#include <cent/str/util.hpp>
#include <string_view>

namespace cent {

constexpr std::string_view strip_left(std::string_view s) {
    while (!s.empty() && is_space(s.front())) s.remove_prefix(1);
    return s;
}

constexpr std::string_view strip_right(std::string_view s) {
    while (!s.empty() && is_space(s.back())) s.remove_suffix(1);
    return s;
}

constexpr std::string_view strip(std::string_view s) {
    return strip_left(strip_right(s));
}

constexpr std::string_view strip_left(std::string_view s, char c) {
    while (!s.empty() && s.front() == c) s.remove_prefix(1);
    return s;
}

constexpr std::string_view strip_right(std::string_view s, char c) {
    while (!s.empty() && s.back() == c) s.remove_suffix(1);
    return s;
}

constexpr std::string_view strip(std::string_view s, char c) {
    return strip_left(strip_right(s, c), c);
}

}  // namespace cent
