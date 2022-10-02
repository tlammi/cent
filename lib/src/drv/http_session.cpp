#include "cent/drv/container.hpp"
#include "cent/drv/http_session_impl.hpp"
#include "cent/raise.hpp"

namespace cent::drv {
namespace {

HttpSessionFunc g_http{};

auto& http_container() { return g_container<HttpSessionFunc>; }

}  // namespace

void register_http_session(std::string name, const HttpSessionFunc& func) {
    http_container()[std::move(name)] = func;
}
std::vector<std::string_view> list_http_sessions() {
    return container_names(http_container());
}

void set_http_session(std::string_view name) {
    auto iter = http_container().find(name);
    if (iter == http_container().end())
        raise("Invalid HTTP session driver name: ", name);
    g_http = iter->second;
}

std::unique_ptr<HttpSession> http_session() { return g_http(); }

}  // namespace cent::drv