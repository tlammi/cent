/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <vector>

#include "cent/digest.hpp"
#include "cent/drv/file_system.hpp"
#include "cent/lock_file.hpp"
#include "cent/reference.hpp"

namespace cent {

class Storage {
 public:
    Storage(drv::FileSystem* fs, const stdfs::path& root);

    /**
     * Check if a layer with the given digest exists
     */
    bool layer_exists(DigestView digest) const;

    /**
     * Check if a config with the given digest exists
     */
    bool config_exists(DigestView digest) const;

    /// Check if a manifest with the given digest exists
    bool manifest_exists(DigestView digest) const;

    /// Path to file where the layer is stored
    stdfs::path layer_path(DigestView digest);

    /// Open a layer file for reading
    std::unique_ptr<std::iostream> read_layer(DigestView digest);

    /**
     * Open a layer file for writing
     *
     * The layer file is cleared if it already exist (althougth the data should
     * not normally change as the digest is calculated from the contents)
     */
    std::unique_ptr<std::iostream> write_layer(DigestView digest);

    /**
     * Open config file for reading
     */
    std::unique_ptr<std::iostream> read_config(DigestView digest);

    /**
     * Open (and create) config file for writing
     */
    std::unique_ptr<std::iostream> write_config(DigestView digest);

    std::unique_ptr<std::iostream> read_manifest(DigestView digest);
    std::unique_ptr<std::iostream> write_manifest(DigestView digest);

    Digest lookup_manifest(const Reference& image);
    void store_image_name(DigestView manifest, const Reference& image);

    std::vector<Reference> list_images() const;

 private:
    LockFile m_lk{};
    drv::FileSystem* m_fs;
    stdfs::path m_root;
};
}  // namespace cent