#pragma once

#include <cent/dt/result.hpp>
#include <cent/json/json.hpp>
#include <cent/json/lex.hpp>
#include <cent/util/str.hpp>
#include <string_view>

namespace cent::json {

Result<Json> parse(std::string_view s);

}  // namespace cent::json
