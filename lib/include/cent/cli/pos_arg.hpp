#pragma once

#include <cent/cli/value.hpp>
#include <memory>
#include <string_view>

namespace cent::cli {

struct PosArg {
    std::string_view name;
    std::unique_ptr<IValue> value;
    std::string_view help;
};
}  // namespace cent::cli
