/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <optional>
#include <vector>

#include "cent/def.hpp"
#include "cent/drv/driver.hpp"
#include "cent/result.hpp"

namespace cent::drv {

/**
 * Interface for libcent to access the file system
 */
class FileSystem : public Driver {
 public:
    /**
     * Make a directory
     *
     * Relative paths are resolved relative to the current working directory.
     *
     * If parents is true, acts like mkdir -p so already existing directory
     * is not an error.
     *
     * \param path Path to the directory to create
     * \param parents Create all parent directories if they do not exist
     * \return Result
     */
    virtual Result mkdir(const stdfs::path& path, bool parents = false) = 0;

    virtual std::unique_ptr<std::iostream> open_file(
        const stdfs::path& path,
        std::ios_base::openmode mode = std::ios_base::in |
                                       std::ios_base::out) = 0;
    virtual bool exists(const stdfs::path& path) = 0;

    /**
     * Find a given program from the filesystem
     *
     * \param program Program name to search
     *
     * \return Absolute path to match, or "" if not found.
     */
    virtual stdfs::path find_program(std::string_view program) = 0;

    /**
     * Lock a file and create one if it does not exist
     *
     * \param path Path to lock file to aqcuire
     * \return File descriptor to lock file
     */
    virtual int lock_file(const stdfs::path& path) = 0;
    /**
     * Unlock a previously locked file
     *
     * This is guaranteed to be called once for each lock_file call.
     *
     * \param handle Lock file handle
     */
    virtual void unlock_file(int fd) = 0;

    /**
     * Peform an overlay mount
     *
     * \param paths Read-only layers to mount
     * \param upper Upper dir for the mount. "" to skip
     * \param work Work dir for the mount. "" to skp
     * \param dst Destination path where to mount
     *
     */
    virtual void overlayfs_mount(const std::vector<stdfs::path>& lowers,
                                 const stdfs::path& upper,
                                 const stdfs::path& work,
                                 const stdfs::path& dst) = 0;
};

void register_file_system(std::string name, FileSystem* fs);
std::vector<std::string_view> list_file_systems();

}  // namespace cent::drv