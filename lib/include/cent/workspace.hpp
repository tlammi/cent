#pragma once

#include "cent/digest.hpp"
#include "cent/drv/drivers.hpp"

namespace cent {

class Workspace {
 public:
    Workspace(drv::FileSystem* fs, const stdfs::path& root);

    /**
     * Creates a directory for extrating a layer fs.
     *
     * This creates an empty layer directory.
     * An error is throw in case the layer already exists.
     *
     * \param digest Layer digest to create
     * \return Path to the layer
     */
    stdfs::path create_layer(DigestView digest);

    /**
     * Check if the given layer exists in the workspace
     *
     * \param digest Layer digest
     * \return True if the layer exists, false, if not
     */
    bool layer_exists(DigestView digest);

 private:
    drv::FileSystem* m_fs;
    stdfs::path m_root;
};
}  // namespace cent