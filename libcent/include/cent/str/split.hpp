#pragma once

#include <concepts>
#include <string_view>

namespace cent {
namespace split_detail {

constexpr size_t tok_size(std::string_view tok) { return tok.size(); }
constexpr size_t tok_size(char /*tok*/) { return 1; }

}  // namespace split_detail

template <class Tok>
constexpr std::pair<std::string_view, std::string_view> split_left(
    std::string_view str, Tok tok) {
    auto pos = str.find(tok);
    if (pos == std::string_view::npos) { return {str, {}}; }
    return {str.substr(0, pos), str.substr(pos + split_detail::tok_size(tok))};
}

}  // namespace cent
