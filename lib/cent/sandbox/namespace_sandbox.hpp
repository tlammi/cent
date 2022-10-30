#pragma once

#include <cent/sandbox/driver.hpp>

namespace cent::sandbox {
std::shared_ptr<Driver> make_namespace_sandbox_driver();
}  // namespace cent::sandbox