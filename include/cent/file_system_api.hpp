#pragma once

#include <filesystem>
#include <istream>
#include <memory>

#include "cent/result.hpp"

namespace cent {

namespace stdfs = std::filesystem;

/**
 * Interface for libcent to access the file system
 */
class FileSystemApi {
 public:
    virtual ~FileSystemApi() {}

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
};

std::unique_ptr<FileSystemApi> default_file_system_api();
}  // namespace cent