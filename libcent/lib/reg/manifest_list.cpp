#include <cent/bits/reg/manifest_list.hpp>

namespace cent::reg {
namespace {
namespace manifest_list_detail {}
}  // namespace

json::Obj to_json(const Platform& p) {
    json::Obj res{};
    if (!p.arch.empty()) res["architecture"] = json::Json(p.arch);
    if (!p.os.empty()) res["os"] = json::Json(p.os);
    if (!p.variant.empty()) res["variant"] = json::Json(p.variant);
    return res;
}

json::Obj to_json(const ManifestListEntry& e) {
    json::Obj res{};
    if (!e.digest.empty()) res["digest"] = json::Json(e.digest);
    auto platform = to_json(e.platform);
    if (!platform.empty()) res["platform"] = json::Json(std::move(platform));
    res["size"] = json::Json(static_cast<json::Int>(e.size));
    return res;
}

json::Obj to_json(const ManifestList& ml) {
    json::Obj out{};
    json::Arr& arr = (out["manifest"] = json::Json()).as_arr();
    for (const auto& m : ml) { arr.push_back(json::Json(to_json(m))); }
    return out;
}

std::ostream& operator<<(std::ostream& os, const Platform& p);
std::ostream& operator<<(std::ostream& os, const ManifestListEntry& e);
std::ostream& operator<<(std::ostream& os, const ManifestList& ml);

Result<ManifestList> make_manifest_list(const json::Json& jsn) {
    ManifestList res{};
    for (auto& arr_entry : jsn["manifests"].as_arr()) {
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
