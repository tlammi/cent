#include "cent/fs.hpp"

#include <cent/fs/driver.hpp>

#include "linux_driver.hpp"

namespace cent::fs {
namespace {
std::shared_ptr<cent::fs::Driver> g_drv{};
}

void set_driver(std::shared_ptr<cent::fs::Driver> drv) {
    g_drv = std::move(drv);
}
std::vector<std::string_view> builtin_drivers() { return {"linux"}; }
std::shared_ptr<Driver> builtin_driver(std::string_view name) {
    if (name == "linux") return make_linux_driver();
    return nullptr;
}
std::error_code mkdir(const stdfs::path& path, bool parents) {
    return g_drv->mkdir(path, parents);
}

std::unique_ptr<std::iostream> open_file(const stdfs::path& path,
                                         std::ios_base::openmode mode) {
    return g_drv->open_file(path, mode);
}

bool exists(const stdfs::path& path) { return g_drv->exists(path); }
std::error_code rm(const stdfs::path& path, bool recurse, bool force) {
    return g_drv->rm(path, recurse, force);
}

stdfs::path find_program(std::string_view program) {
    return g_drv->find_program(program);
}

int run_program(const stdfs::path& cmd,
                const std::vector<std::string_view>& args,
                const std::vector<std::string_view>& env, std::string* out,
                std::string* err, const std::string* in) {
    return g_drv->run_program(cmd, args, env, out, err, in);
}

int lock_file(const stdfs::path& path) { return g_drv->lock_file(path); }

void unlock_file(int fd) { return g_drv->unlock_file(fd); }

void overlayfs_mount(const std::vector<stdfs::path>& lowers,
                     const stdfs::path& upper, const stdfs::path& work,
                     const stdfs::path& dst) {
    g_drv->overlayfs_mount(lowers, upper, work, dst);
}

}  // namespace cent::fs
