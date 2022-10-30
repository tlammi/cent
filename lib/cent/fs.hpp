#pragma once

#include <cent/fs/driver.hpp>

#include "cent/fs/lock_file.hpp"

namespace cent::fs {

std::error_code mkdir(const stdfs::path& path, bool parents = false);

std::unique_ptr<std::iostream> open_file(
    const stdfs::path& path,
    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

bool exists(const stdfs::path& path);

std::error_code rm(const stdfs::path& path, bool recurse, bool force);

stdfs::path find_program(std::string_view program);

int run_program(const stdfs::path& cmd,
                const std::vector<std::string_view>& args,
                const std::vector<std::string_view>& env, std::string* out,
                std::string* err, const std::string* in);

int lock_file(const stdfs::path& path);

void unlock_file(int fd);

void overlayfs_mount(const std::vector<stdfs::path>& lowers,
                     const stdfs::path& upper, const stdfs::path& work,
                     const stdfs::path& dst);

}  // namespace cent::fs