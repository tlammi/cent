#pragma once

namespace cent {

constexpr char to_lower(char c) noexcept {
    if (c >= 'A' && c <= 'Z')
        return static_cast<char>(static_cast<char>(c - 'A') + 'a');
    return c;
}

constexpr bool is_space(char c) noexcept {
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\v':
        case '\f':
        case '\r': return true;
        default: return false;
    }
}

}  // namespace cent
