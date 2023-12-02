#pragma once

namespace cent {

constexpr char to_lower(char c) noexcept {
    if (c >= 'A' && c <= 'Z')
        return static_cast<char>(static_cast<char>(c - 'A') + 'a');
    return c;
}

}  // namespace cent
