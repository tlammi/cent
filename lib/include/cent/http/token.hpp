/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <nlohmann/json.hpp>
#include <optional>

namespace cent::http {

struct Token {
    std::string token;

    static std::optional<Token> parse(std::string_view str) {
        auto json = nlohmann::json::parse(str);
        if (json.contains("token")) { return Token{json["token"]}; }
        return std::nullopt;
    }
};
}  // namespace cent::http