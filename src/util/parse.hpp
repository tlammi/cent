#pragma once

#include <concepts>
#include <optional>
#include <string_view>

namespace cent::util {

template <std::integral I>
constexpr std::optional<I> parse_int(std::string_view s) {
    static constexpr auto is_unsigned = std::unsigned_integral<I>;
    if (s.empty()) return std::nullopt;
    bool neg = s.front() == '-';
    if (neg) {
        if (is_unsigned) return std::nullopt;
        s.remove_prefix(1);
    }
    I out{};
    for (auto c : s) {
        out *= 10;
        if (c >= '0' && c <= '9')
            out += c - '0';
        else
            return std::nullopt;
    }
    return neg ? -out : out;
}

}  // namespace cent::util
