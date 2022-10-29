#pragma once

#include <cent/driver.hpp>
#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

namespace cent::fs {
namespace stdfs = std::filesystem;

class Driver : public ::cent::Driver {
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
    virtual std::error_code mkdir(const stdfs::path& path,
                                  bool parents = false) = 0;

    virtual std::unique_ptr<std::iostream> open_file(
        const stdfs::path& path,
        std::ios_base::openmode mode = std::ios_base::in |
                                       std::ios_base::out) = 0;

    virtual bool exists(const stdfs::path& path) = 0;

    /**
     * Remove files/directories like "rm" util
     *
     * \param path what to delete
     * \param recurse Like "-r" flag for "rm" command
     * \param force Like "-f" flag for "rm" command
     *
     * \return std::error_code describing the result
     */
    virtual std::error_code rm(const stdfs::path& path, bool recurse,
                               bool force) = 0;

    /**
     * Find a given program from the filesystem
     *
     * \param program Program name to search
     *
     * \return Absolute path to match, or "" if not found.
     */
    virtual stdfs::path find_program(std::string_view program) = 0;

    /**
     * Run a given program with the given settings
     *
     * \param cmd Path to the executable
     * \param args List of arguments to pass to the callable
     * \param env List of environment variables in format variable=value to pass
     * to the executable
     * \param out Where to store stdout. Nullptr if ignored
     * \param err Where to store stderr. Nullptr if ignored
     * \param in Stdin to pass to the command. Nullptr means that nothing is
     * piped. "" means that pipe is opened but immediately closed.
     *
     * \return Executable return value
     */
    virtual int run_program(const stdfs::path& cmd,
                            const std::vector<std::string_view>& args,
                            const std::vector<std::string_view>& env,
                            std::string* out, std::string* err,
                            const std::string* in) = 0;

    /**
     * Lock a file and create one if it does not exist
     *
     * \param path Path to lock file to aqcuire
     * \return File descriptor to lock file or 0 on error
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

std::vector<std::string_view> builtin_drivers();
std::shared_ptr<Driver> builtin_driver(std::string_view name);
void set_driver(std::shared_ptr<Driver> drv);

}  // namespace cent::fs