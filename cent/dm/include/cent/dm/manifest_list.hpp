#pragma once

#include <cent/dm/digest.hpp>
#include <cent/dm/mime.hpp>
#include <cent/dm/platform.hpp>
#include <nlohmann/json.hpp>

namespace cent::dm {

class ManifestList {
 public:
    struct Entry {
        Digest digest;
        MimeType media_type;
        Platform platform;
        size_t size;
    };
    ManifestList() = default;
    explicit ManifestList(const nlohmann::json& data);

    const std::vector<Entry>& entries() const noexcept;
    MimeType media_type() const noexcept;
    size_t schema_version() const noexcept;

    const Entry* platform_manifest(const Platform& platform) const;

 private:
    std::vector<Entry> m_entries{};
    MimeType m_media_type{};
    size_t m_schema_version{0};
};

}  // namespace cent::dm

template <>
struct fmt::formatter<cent::dm::ManifestList> : fmt::formatter<std::string> {
    auto format(const cent::dm::ManifestList& m, fmt::format_context& ctx) {
        return fmt::formatter<std::string>::format(
            "ManifestList{<placeholder>}", ctx);
    }
};

