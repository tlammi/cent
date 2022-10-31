
#include "cent/net.hpp"

#include <cent/net/driver.hpp>

#include "curl_driver.hpp"
#include "fs_driver.hpp"

namespace cent::net {
namespace {
std::shared_ptr<Driver> g_drv{nullptr};
}
std::vector<std::string_view> builtin_drivers() { return {"curl"}; }
std::shared_ptr<Driver> builtin_driver(std::string_view name) {
    if (name == "curl") { return make_curl_driver(); }
    if (name == "fs") { return make_fs_driver(); }
    return nullptr;
}
void set_driver(std::shared_ptr<Driver> drv) { g_drv = std::move(drv); }

std::unique_ptr<HttpSession> http_session() { return g_drv->http_session(); }
std::unique_ptr<HttpClient> http_client() {
    return std::make_unique<HttpClient>();
}
}  // namespace cent::net