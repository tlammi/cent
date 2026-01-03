#pragma once

#include <string>
#include <vector>

#include "io/stream.hpp"

namespace cent::component {

class Storage {
 public:
    virtual ~Storage() = default;

    /**
     * \brief Read manfifest
     * */
    virtual std::string manifest(std::string_view digest) = 0;

    /**
     * \brief Write manifest
     * */
    virtual void manifest_set(std::string_view digest,
                              std::string_view value) = 0;
    /**
     * \brief Remove manifest
     *
     * \return true if found (and removed)
     * */
    virtual bool manifest_rm(std::string_view digest) = 0;

    /**
     * \brief List the stored manifests
     * */
    virtual std::vector<std::string> manifest_ls() = 0;

    /**
     * \brief Read config
     * */
    virtual std::string config(std::string_view digest) = 0;

    /**
     * \brief Write config
     * */
    virtual void config_set(std::string_view digest,
                            std::string_view value) = 0;

    /**
     * \brief Remove config
     *
     * \return true if found (and removed)
     * */
    virtual bool config_rm(std::string_view digest) = 0;

    /**
     * \brief List configs
     * */
    virtual std::vector<std::string> config_ls() = 0;

    /**
     * \brief Write a layer
     * */
    virtual io::AnyOStream layer_write(std::string_view digest) = 0;

    /**
     * \brief Read a layer
     * */
    virtual io::AnyIStream layer_read(std::string_view digest) = 0;

    /**
     * \brief Remove a layer
     *
     * \return true if found (and removed)
     * */
    virtual bool layer_rm(std::string_view digest) = 0;

    /**
     * \brief List layers
     * */
    virtual std::vector<std::string> layer_ls() = 0;

    /**
     * \brief Migrate from one storage to another
     *
     * TODO: Write better spec
     * */
    virtual void migrate(Storage& other) = 0;
};

}  // namespace cent::component
