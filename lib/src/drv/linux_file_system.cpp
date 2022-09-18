/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include <sys/file.h>
#include <unistd.h>

#include <fstream>

#include "cent/drv/file_system.hpp"
#include "cent/raise.hpp"

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
};

std::unique_ptr<FileSystem> default_file_system() {
    return std::make_unique<LinuxFileSystemApi>();
}
}  // namespace cent::drv