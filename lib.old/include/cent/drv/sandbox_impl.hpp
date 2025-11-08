#pragma once

#include "cent/drv/sandbox.hpp"

namespace cent::drv {

void set_sandbox(std::string_view name);

std::unique_ptr<Sandbox> sandbox();
std::unique_ptr<Sandbox> default_sandbox();
}  // namespace cent::drv