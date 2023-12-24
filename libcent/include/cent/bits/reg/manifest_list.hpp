#pragma once

#include <cent/json.hpp>
#include <string>
#include <vector>

namespace cent::reg {

struct Platform {
    std::string arch;
    std::string os;
    std::string variant;
};

struct ManifestListEntry {
    std::string digest;
    Platform platform;
    size_t size;
};

using ManifestList = std::vector<ManifestListEntry>;

json::Obj to_json(const Platform& p);
json::Obj to_json(const ManifestListEntry& e);
json::Obj to_json(const ManifestList& ml);

std::ostream& operator<<(std::ostream& os, const Platform& p);
std::ostream& operator<<(std::ostream& os, const ManifestListEntry& e);
std::ostream& operator<<(std::ostream& os, const ManifestList& ml);

Result<ManifestList> make_manifest_list(const json::Json& jsn);

}  // namespace cent::reg
