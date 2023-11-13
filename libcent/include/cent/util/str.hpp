#pragma once

#include <cent/dt/result.hpp>
#include <cent/util/str_concat.hpp>
#include <charconv>

namespace cent {

template <std::integral T>
constexpr Result<T> parse_int(std::string_view s) {
    T out{};
    bool neg = false;
    if (s.starts_with('-')) {
        neg = true;
        s.remove_prefix(1);
    }
    if (s.starts_with("0x")) {
        s.remove_prefix(2);
        for (auto c : s) {
            if (c >= '0' && c <= '9') {
                out *= 16;
                out += (c - '0');
            } else if (c >= 'a' && c <= 'f') {
                out *= 16;
                out += (c - 'a' + 10);
            } else if (c >= 'A' && c <= 'F') {
                out *= 16;
                out += (c - 'A' + 10);
            }
        }
    } else if (s.starts_with("0b")) {
        s.remove_prefix(2);
        for (auto c : s) {
            out <<= 1;
            out += (c - '0');
        }
    } else {
        for (auto c : s) {
            out *= 10;
            out += (c - '0');
        }
    }
    return neg ? -out : out;
}

template <std::floating_point T>
Result<T> parse_float(std::string_view s) {
    T out{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), out);
    if (ec == std::errc()) { return out; }
    return Result<T>(ec);
}

}  // namespace cent
