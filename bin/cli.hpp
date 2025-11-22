#pragma once

#include <variant>

namespace cent::bin {

struct Pull {};

struct Cli {
    std::variant<Pull> cmd{};
};

Cli parse_cli(int argc, char** argv);

}  // namespace cent::bin
