#pragma once

#include <string_view>

#include "cent/def.hpp"

namespace cent {

struct Drivers {
    std::string file_system{"default"};
    std::string http_session{"default"};
    std::string sandbox{"default"};
    std::string unpacker{"default"};
};

/**
 * Application configuration
 */
struct Config {
    Drivers drivers{};
    stdfs::path storage_path{stdfs::path{std::getenv("HOME")} /
                             ".cent/storage"};
    stdfs::path workspace_path{stdfs::path{std::getenv("HOME")} /
                               ".cent/workspace"};
};
}  // namespace cent