#pragma once

#include <string_view>
#include <variant>

namespace cent::bin {

struct Pull {
    std::string_view image;
};

struct Cli {
    std::variant<Pull> cmd{};
};

Cli parse_cli(int argc, char** argv);

}  // namespace cent::bin
