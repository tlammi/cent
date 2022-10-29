
#pragma once
#include <cent/fs/driver.hpp>

namespace cent::fs {

std::shared_ptr<Driver> make_linux_driver();

}  // namespace cent::fs