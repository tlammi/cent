/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/strutil.hpp"

#include <tuple>

namespace cent {
std::pair<std::string_view, std::string_view> split_left(
    std::string_view str, std::string_view token) {
    auto idx = str.find(token);
    if (idx == std::string_view::npos) return {str, ""};
    auto first = str.substr(0, idx);
    auto second = str.substr(idx + token.size());
    return {first, second};
}

std::vector<std::string_view> split(std::string_view str,
                                    std::string_view token) {
    std::vector<std::string_view> res{};
    std::string_view next{};
    while (true) {
        std::tie(next, str) = split_left(str, token);
        res.push_back(next);
        if (str.empty()) break;
    }
    return res;
}

}  // namespace cent