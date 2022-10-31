#pragma once

#include <cent/net/driver.hpp>

namespace cent::net {

std::shared_ptr<Driver> make_fs_driver();
}  // namespace cent::net