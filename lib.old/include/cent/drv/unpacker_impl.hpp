#pragma once

#include "cent/drv/unpacker.hpp"
namespace cent::drv {

std::unique_ptr<Unpacker> default_unpacker();

void set_unpacker(std::string_view name);
Unpacker& unpacker();

}  // namespace cent::drv