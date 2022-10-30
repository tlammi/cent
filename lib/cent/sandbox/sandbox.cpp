#include <cent/sandbox/driver.hpp>
#include <string_view>

#include "namespace_sandbox.hpp"

namespace cent::sandbox {
namespace {
std::shared_ptr<Driver> g_drv{nullptr};
}
std::vector<std::string_view> builtin_drivers() { return {"namespace"}; }
std::shared_ptr<Driver> builtin_driver(std::string_view name) {
    if (name == "namespace") { return make_namespace_sandbox_driver(); }
    return nullptr;
}

void set_driver(std::shared_ptr<Driver> drv) { g_drv = std::move(drv); }

std::shared_ptr<Sandbox> sandbox() { return g_drv->sandbox(); }
}  // namespace cent::sandbox