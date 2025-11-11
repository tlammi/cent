#pragma once

#include <string_view>
#include <utility>

namespace cent::util {

template <class Tok>
constexpr auto split_first(std::string_view str, Tok tok) {
    auto idx = str.find(tok);
    if (idx == std::string_view::npos)
        return std::pair{str, std::string_view{}};
    if constexpr (std::same_as<Tok, char>)
        return std::pair{str.substr(0, idx), str.substr(idx + 1)};
    else
        return std::pair{str.substr(0, idx), str.substr(idx + tok.size())};
}

}  // namespace cent::util
