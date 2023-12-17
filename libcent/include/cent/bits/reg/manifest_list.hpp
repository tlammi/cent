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

inline Result<ManifestList> make_manifest_list(const json::Json& jsn) {
    ManifestList res{};
    for (auto& arr_entry : jsn.as_obj()["manifests"].as_arr()) {
        auto& entry = arr_entry.as_obj();
        ManifestListEntry e{};
        e.digest = entry["digest"].as_str();
        e.size = entry["size"].as_int();
        auto& platform = entry["platform"].as_obj();
        if (platform.contains("architecture"))
            e.platform.arch = platform["architecture"].as_str();
        if (platform.contains("os")) e.platform.os = platform["os"].as_str();
        if (platform.contains("variant"))
            e.platform.variant = platform["variant"].as_str();
        res.push_back(e);
    }
    return res;
}

}  // namespace cent::reg
