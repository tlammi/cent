#pragma once

#include <cent/archive/driver.hpp>

namespace cent::archive {

std::shared_ptr<Driver> make_libarchive_driver();

}