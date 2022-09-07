#include <fstream>

#include "cent/file_system_api.hpp"

namespace cent {

class LinuxFileSystemApi final : public FileSystemApi {
 public:
    Result mkdir(const stdfs::path& path, bool parents) override {
        std::error_code ec;
        if (parents) {
            bool res = stdfs::create_directories(path, ec);
            if (!res) return Result{ec.value(), ec.message()};
            return Result{0, ""};
        }
        bool res = stdfs::create_directory(path, ec);
        if (!res) return Result{ec.value(), ec.message()};
        return Result{0, ""};
    }

    std::unique_ptr<std::iostream> open_file(
        const stdfs::path& path, std::ios_base::openmode mode) override {
        return std::make_unique<std::fstream>(path, mode);
    }
};

std::unique_ptr<FileSystemApi> default_file_system_api() {
    return std::make_unique<LinuxFileSystemApi>();
}
}  // namespace cent