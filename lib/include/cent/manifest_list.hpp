/* SPDX-License-Identifier:  GPL-3.0-or-later */
#pragma once

#include <nlohmann/json.hpp>
#include <ostream>
#include <vector>

#include "cent/digest.hpp"
#include "cent/media_type.hpp"
#include "cent/platform.hpp"

namespace cent {
class ManifestList {
 public:
    struct Entry {
        Digest digest;
        MediaType media_type;
        Platform platform;
        size_t size;
    };
    ManifestList();
    explicit ManifestList(const nlohmann::json& json);

    const std::vector<Entry>& manifests() const noexcept;
    MediaType media_type() const noexcept;
    size_t schema_version() const noexcept;

    const Entry* platform_manifest(const Platform& platform) const;

 private:
    std::vector<Entry> m_enries{};
    MediaType m_media_type{
        MediaType::from_kind(MediaKind::DockerManifestListV2)};
    size_t m_schema_version{};
};

std::ostream& operator<<(std::ostream& os, const ManifestList& mlist);
}  // namespace cent