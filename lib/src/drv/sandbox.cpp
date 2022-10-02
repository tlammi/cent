#include "cent/drv/container.hpp"
#include "cent/drv/sandbox_impl.hpp"
#include "cent/raise.hpp"

namespace cent::drv {
namespace {
SandboxFunc g_sandbox{};

auto& sb_container() { return g_container<SandboxFunc>; }

struct Init {
    Init() {
        sb_container()["default"] = default_sandbox;
        g_sandbox = default_sandbox;
    }
};

Init init{};

}  // namespace
void register_sandbox(std::string name, const SandboxFunc& func) {
    sb_container()[std::move(name)] = func;
}
std::vector<std::string_view> list_sandboxes() {
    return container_names(sb_container());
}

void set_sandbox(std::string_view name) {
    auto iter = sb_container().find(name);
    if (iter == sb_container().end()) raise("Invalid sandbox driver: ", name);
    g_sandbox = iter->second;
}

std::unique_ptr<Sandbox> sandbox() { return g_sandbox(); }
}  // namespace cent::drv