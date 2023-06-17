#include <cent/dm/manifest_list.hpp>

#include "cent/dm/mime.hpp"

namespace cent::dm {

ManifestList::ManifestList(const nlohmann::json& data)
    : m_media_type(
          mime_from_str(data["mediaType"].get_ref<const std::string&>())
              .value()),
      m_schema_version(data["schemaVersion"].get<size_t>()) {
    for (const auto& manifest : data["manifests"]) {
        Digest digest{manifest["digest"].get<std::string>()};
        MimeType mtype =
            mime_from_str(manifest["mediaType"].get_ref<const std::string&>())
                .value();

        const auto& pform = manifest["platform"];
        std::string arch = pform.value("architecture", "");
        std::string os = pform.value("os", "");
        std::string variant = pform.value("variant", "");
        Platform platform{std::move(arch), std::move(os), std::move(variant)};

        m_entries.emplace_back(std::move(digest), std::move(mtype),
                               std::move(platform),
                               manifest["size"].get<size_t>());
    }
}

const std::vector<ManifestList::Entry>& ManifestList::entries() const noexcept {
    return m_entries;
}

MimeType ManifestList::media_type() const noexcept { return m_media_type; }
size_t ManifestList::schema_version() const noexcept {
    return m_schema_version;
}

const ManifestList::Entry* ManifestList::platform_manifest(
    const Platform& platform) const {
    auto match = std::find_if(
        m_entries.begin(), m_entries.end(),
        [&](const auto& manifest) { return manifest.platform == platform; });

    if (match != m_entries.end()) { return &*match; }
    return nullptr;
}

}  // namespace cent::dm
