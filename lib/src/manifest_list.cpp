#include "cent/manifest_list.hpp"

namespace cent {

ManifestList::ManifestList() {}
ManifestList::ManifestList(const nlohmann::json& json) : ManifestList() {
    for (const auto& manifest : json["manifests"]) {
        Digest digest{manifest["digest"].get<std::string>()};
        MediaType mtype{
            MediaType::from_mime(manifest["mediaType"].get<std::string>())};

        const auto& pform = manifest["platform"];
        std::string arch = pform.value("architecture", "");
        std::string os = pform.value("os", "");
        std::string variant = pform.value("variant", "");
        Platform platform{std::move(arch), std::move(os), std::move(variant)};

        m_enries.emplace_back(std::move(digest), std::move(mtype),
                              std::move(platform),
                              manifest["size"].get<size_t>());
    }
    m_media_type = MediaType::from_mime(json["mediaType"].get<std::string>());
    m_schema_version = json["schemaVersion"].get<size_t>();
}

auto ManifestList::manifests() const noexcept -> const std::vector<Entry>& {
    return m_enries;
}
MediaType ManifestList::media_type() const noexcept { return m_media_type; }
size_t ManifestList::schema_version() const noexcept {
    return m_schema_version;
}
}  // namespace cent