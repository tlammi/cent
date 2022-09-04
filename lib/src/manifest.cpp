#include "cent/manifest.hpp"

namespace cent {

Manifest::Manifest() {}
Manifest::Manifest(const nlohmann::json& json) {}
MediaType Manifest::media_type() const noexcept {}
size_t Manifest::schema_version() const noexcept {}
auto Manifest::config() const noexcept -> const Config& {}
auto Manifest::layers() const noexcept -> const std::vector<Layer>& {}
}  // namespace cent
