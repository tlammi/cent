/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include <sys/mount.h>
#include <unistd.h>

#include <fstream>

#include "cent/drv/file_system.hpp"
#include "cent/logs.hpp"
#include "cent/raise.hpp"
#include "cent/strutil.hpp"

namespace cent::drv {

class LinuxFileSystemApi final : public FileSystem {
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

    bool exists(const stdfs::path& path) override {
        return stdfs::exists(path);
    }

    int lock_file(const stdfs::path& path) override {
        int fd = open(path.c_str(), O_RDWR | O_CREAT, 0666);
        int rc = flock(fd, LOCK_EX);
        if (rc) { raise("Could not lock", path); }
        return fd;
    }

    void unlock_file(int fd) override { close(fd); }

    void overlayfs_mount(const std::vector<stdfs::path>& paths,
                         const stdfs::path& upper, const stdfs::path& work,
                         const stdfs::path& dst) final {
        std::string options{"lowerdir="};
        options += join(paths, ":");
        if (!upper.empty()) {
            options += ",upperdir=";
            options += upper;
        }
        if (!work.empty()) {
            options += ",workdir=";
            options += work;
        }
        logs::debug("using options: ", options);
        int res = mount("overlay", dst.c_str(), "overlay", 0, options.c_str());
        if (res) raise(strerror(errno));
    }
};

std::unique_ptr<FileSystem> default_file_system() {
    return std::make_unique<LinuxFileSystemApi>();
}
}  // namespace cent::drv