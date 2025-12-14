#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cent {

/**
 * \brief Parsed manifest
 *
 * Minimal information parsed from the message received from the server.
 * */
struct Manifest {
    // Original digest of the document received from the server
    std::string digest;
    // Digest of the config
    std::string config;
    // Digests of the layers
    std::vector<std::string> layers{};
    // Image annotations
    std::map<std::string, std::string> annotations{};
};

}  // namespace cent
