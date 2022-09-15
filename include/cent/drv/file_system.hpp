#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <optional>

#include "cent/result.hpp"

namespace cent {

namespace stdfs = std::filesystem;

namespace drv {
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
};

std::unique_ptr<FileSystem> default_file_system();
}  // namespace drv
}  // namespace cent