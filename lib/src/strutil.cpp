#include "cent/strutil.hpp"

namespace cent {
std::pair<std::string_view, std::string_view> split_left(
    std::string_view str, std::string_view token) {
    auto idx = str.find(token);
    if (idx == std::string_view::npos) return {str, ""};
    auto first = str.substr(0, idx);
    auto second = str.substr(idx + token.size());
    return {first, second};
}

}  // namespace cent