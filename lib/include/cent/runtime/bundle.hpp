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
     * Initialize a bundle in the given path
     *
     * If a bundle already exists it is overwritten
     */
    Bundle(stdfs::path dir, const std::vector<stdfs::path>& layers);

    /**
     * Get a reference to an existing bundle
     */
    explicit Bundle(stdfs::path dir);

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