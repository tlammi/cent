#include "cent/drv/container.hpp"
#include "cent/drv/http_session_impl.hpp"
#include "cent/raise.hpp"

namespace cent::drv {
namespace {

HttpSessionFunc g_http{};

auto& http_container() { return g_container<HttpSessionFunc>; }

struct Global {
    Global() {
        auto lambda = []() { return default_http_session(); };
        http_container()["default"] = lambda;
        g_http = lambda;
    }
};

Global global{};

}  // namespace
void register_http_session(std::string name, const HttpSessionFunc& func) {
    http_container()[std::move(name)] = func;
}
std::vector<std::string_view> list_http_sessions() {
    std::vector<std::string_view> out{};
    out.reserve(http_container().size());
    for (const auto& [k, _] : http_container()) { out.push_back(k); }
    return out;
}

void set_http_session(std::string_view name) {
    auto iter = http_container().find(name);
    if (iter == http_container().end())
        raise("Invalid HTTP session driver name: ", name);
    g_http = iter->second;
}

std::unique_ptr<HttpSession> http_session() { return g_http(); }

}  // namespace cent::drv