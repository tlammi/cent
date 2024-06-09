#pragma once

#include <cent/cli/value.hpp>
#include <memory>

namespace cent::cli {

struct Opt {
    char shortf;
    std::string_view longf;
    std::unique_ptr<IValue> value;
    std::string_view help;
};
}  // namespace cent::cli
