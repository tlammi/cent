/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <optional>
#include <vector>

#include "cent/def.hpp"
#include "cent/result.hpp"

namespace cent::drv {

/**
 * Interface for libcent to access the file system
 */
class FileSystem {
 public:
    virtual ~FileSystem() {}

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
     * Peform a union mount
     *
     * Mounts the given paths as read-only layers of an unioning file system
     * under \a dst. \a readonly tells if the view mounted in \a dst should be
     * read-only or read-write.
     *
     * This method may create directories next to the target directory
     *
     * \param paths Read-only layers to mount
     * \param dst Destination path where to mount
     * \param readonly Tell if the resulting mount should be read-only
     *
     */
    virtual void union_mount(const std::vector<stdfs::path>& paths,
                             const stdfs::path& dst, bool readonly) = 0;
};

std::unique_ptr<FileSystem> default_file_system();

}  // namespace cent::drv