#include "search_path.hpp"

#include <ranges>

#include "error.hpp"

namespace cent::process {

namespace fs = std::filesystem;

fs::path search_path(std::string_view prog, std::string_view path) {
    for (auto part : path | std::views::split(':')) {
        auto path = std::filesystem::path(std::string_view(part));
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().filename() != prog) continue;
            if (!entry.is_regular_file() && !entry.is_symlink()) continue;
            auto s = fs::status(entry.path());
            using enum fs::perms;
            // TODO: Do a better check if the file is actually executable.
            if ((s.permissions() & (owner_exec | group_exec | others_exec)) ==
                none)
                continue;
            return entry.path();
        }
    }
    return {};
}

fs::path search_path(std::string_view prog) {
    const auto* path = std::getenv("PATH");
    if (!path) return {};
    return search_path(prog, path);
}

}  // namespace cent::process
