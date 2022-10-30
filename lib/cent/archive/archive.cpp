#include "cent/archive.hpp"

#include "libarchive_driver.hpp"

namespace cent::archive {

namespace {
std::shared_ptr<Driver> g_drv{nullptr};
}
void unpack(const stdfs::path& src, const stdfs::path& dst) {
    g_drv->unpack(src, dst);
}

std::vector<std::string_view> builtin_drivers() { return {"libarchive"}; }
std::shared_ptr<Driver> builtin_driver(std::string_view name) {
    if (name == "libarchive") return make_libarchive_driver();
    return nullptr;
}
void set_driver(std::shared_ptr<Driver> drv) { g_drv = std::move(drv); }
}  // namespace cent::archive