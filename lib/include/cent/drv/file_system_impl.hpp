#pragma once

#include "cent/drv/file_system.hpp"

namespace cent::drv {

void set_file_system(std::string_view name);

FileSystem& fs();

}  // namespace cent::drv