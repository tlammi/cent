#include "cent/drv/container.hpp"
#include "cent/drv/file_system_impl.hpp"
#include "cent/raise.hpp"

namespace cent::drv {
namespace {

FileSystem* g_fs = nullptr;  // NOLINT

auto& fs_container() { return g_container<FileSystem*>; }

struct Global {
    Global() {
        fs_container()["default"] = default_fs.get();
        g_fs = default_fs.get();
    }
    std::unique_ptr<FileSystem> default_fs{default_file_system()};
};

Global global{};
}  // namespace
void register_file_system(std::string name, FileSystem* fs) {
    fs_container()[std::move(name)] = fs;
}

std::vector<std::string_view> list_file_systems() {
    return container_names(fs_container());
}

void set_file_system(std::string_view name) {
    auto iter = fs_container().find(name);
    if (iter == fs_container().end())
        raise("Non existent FileSystem driver ", name);
    g_fs = iter->second;
}

FileSystem& fs() {
    if (!g_fs) raise("Unset FileSystem driver");
    return *g_fs;
}

}  // namespace cent::drv