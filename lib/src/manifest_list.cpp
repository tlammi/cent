/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
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

auto ManifestList::platform_manifest(const Platform& platform) const
    -> const Entry* {
    for (const auto& manifest : m_enries) {
        if (platform.architecture == manifest.platform.architecture &&
            platform.os == manifest.platform.os &&
            platform.variant == manifest.platform.variant)
            return &manifest;
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const ManifestList& mlist) {
    nlohmann::json json{};
    json["manifests"] = nlohmann::json::array();
    for (const auto& entry : mlist.manifests()) {
        nlohmann::json mfest{};
        mfest["digest"] = entry.digest.str();
        mfest["mediaType"] = entry.media_type.mime;
        auto& pform = mfest["platform"] = nlohmann::json::object();
        if (!entry.platform.architecture.empty())
            pform["architecture"] = entry.platform.architecture;
        if (!entry.platform.os.empty()) pform["os"] = entry.platform.os;
        if (!entry.platform.variant.empty())
            pform["variant"] = entry.platform.variant;
        mfest["size"] = entry.size;
        json["manifests"].push_back(std::move(mfest));
    }
    json["mediaType"] = mlist.media_type().mime;
    json["schemaVersion"] = mlist.schema_version();
    os << json.dump(4);
    return os;
}

}  // namespace cent