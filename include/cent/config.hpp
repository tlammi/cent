#pragma once

#include <string_view>

#include "cent/def.hpp"

namespace cent {

/**
 * Application configuration
*/
struct Config {
    stdfs::path storage_path{stdfs::path{std::getenv("HOME")} /
                             ".cent/storage"};
    stdfs::path workspace_path{stdfs::path{std::getenv("HOME")} /
                               ".cent/workspace"};
};
}  // namespace cent