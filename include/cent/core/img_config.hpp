#pragma once

#include <cent/platform.hpp>
#include <map>
#include <vector>

namespace cent {
struct ImgConfig {
    Platform platform{};
    struct {
        std::map<std::string, std::string> env{};
        std::vector<std::string> cmd{};
        std::string working_dir{};
    } config;
};

}  // namespace cent
