#pragma once

#include <map>
#include <string>
#include <vector>

namespace cent {

/**
 * \brief Parsed manifest
 *
 * Minimal information parsed from the message received from the server.
 * */
struct Manifest {
    std::string config;
    std::vector<std::string> layers{};
    std::map<std::string, std::string> annotations{};
};

}  // namespace cent
