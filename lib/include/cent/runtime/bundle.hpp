#pragma once

#include <nlohmann/json.hpp>

#include "cent/def.hpp"

namespace cent::runtime {

/**
 * Runtime bundle
 *
 * Contains rootfs and configuration for a container.
 */
class Bundle {
 public:
    /**
     * Initialize a bundle in the given patn
     *
     * The directory is created if it does not exist
     */
    Bundle(stdfs::path dir);

    /// Path to root file system
    stdfs::path root() const;

    /// Read configuration
    nlohmann::json config();

    /// Write config
    void config(const nlohmann::json& config);

    /// Path to config file
    stdfs::path config_file() const;

    const stdfs::path& path() const noexcept;

 private:
    stdfs::path m_path;
};
}  // namespace cent::runtime