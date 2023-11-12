#pragma once

#include <concepts>
#include <string>
#include <string_view>

namespace cent {

template <std::convertible_to<std::string_view>... Ts>
std::string concat(Ts&&... ts) {
    const size_t total_len = (std::string_view(ts).size() + ...);
    std::string out{};
    out.reserve(total_len);
    (out += ... += std::string_view(std::forward<Ts>(ts)));
    return out;
}
}  // namespace cent
