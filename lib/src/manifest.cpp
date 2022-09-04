#include "cent/manifest.hpp"

namespace cent {

Manifest::Manifest() {}
Manifest::Manifest(const nlohmann::json& json) {
    m_schema_version = json["schemaVersion"].get<size_t>();
    m_media_type = MediaType::from_mime(json["mediaType"].get<std::string>());
    const auto& conf = json["config"];
    m_config.media_type =
        MediaType::from_mime(conf["mediaType"].get<std::string>());
    m_config.size = conf["size"].get<size_t>();
    m_config.digest = Digest(conf["digest"].get<std::string>());

    for (const auto& layer : json["layers"])
        m_layers.emplace_back(
            MediaType::from_mime(layer["mediaType"].get<std::string>()),
            layer["size"].get<size_t>(),
            Digest(layer["digest"].get<std::string>()));
}

MediaType Manifest::media_type() const noexcept { return m_media_type; }
size_t Manifest::schema_version() const noexcept { return m_schema_version; }
auto Manifest::config() const noexcept -> const Config& { return m_config; }
auto Manifest::layers() const noexcept -> const std::vector<Layer>& {
    return m_layers;
}
}  // namespace cent
