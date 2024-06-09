#pragma once

#include <string_view>

namespace cent::cli {

struct Flag {
    char shortf;
    std::string_view longf;
    bool* store;
    std::string_view help;
};
}  // namespace cent::cli
