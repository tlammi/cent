#pragma once

#include "cent/digest.hpp"
#include "cent/file_system_api.hpp"

namespace cent {

class Storage {
 public:
    Storage(FileSystemApi* fs, const stdfs::path& root);

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

 private:
    FileSystemApi* m_fs;
    stdfs::path m_root;
};
}  // namespace cent