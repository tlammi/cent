#pragma once

#include <vector>

#include "cent/config.hpp"
#include "cent/types.hpp"

namespace cent {

struct Context {
    explicit Context(const Config& conf);

    stdfs::path storage_path;
    stdfs::path workspace_path;
    std::vector<IdMap> uid_maps;
    std::vector<IdMap> gid_maps;
};
}  // namespace cent