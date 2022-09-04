#pragma once

#include <nlohmann/json.hpp>
#include <vector>

#include "cent/digest.hpp"
#include "cent/media_type.hpp"

namespace cent {
/*
{
   "schemaVersion": 2,
   "mediaType": "application/vnd.docker.distribution.manifest.v2+json",
   "config": {
      "mediaType": "application/vnd.docker.container.image.v1+json",
      "size": 1463,
      "digest":
"sha256:a0ce5a295b637a10bc329ded296a0c895e5e56e7c5e674188d423e213b0d213e"
   },
   "layers": [
      {
         "mediaType": "application/vnd.docker.image.rootfs.diff.tar.gzip",
         "size": 28572685,
         "digest":
"sha256:675920708c8bf10fbd02693dc8f43ee7dbe0a99cdfd55e06e6f1a8b43fd08e3f"
      }
   ]
}
*/
class Manifest {
 public:
    struct Config {
        MediaType media_type;
        size_t size;
        Digest digest;
    };
    struct Layer {
        MediaType media_type;
        size_t size;
        Digest digest;
    };
    Manifest();
    explicit Manifest(const nlohmann::json& json);

    MediaType media_type() const noexcept;
    size_t schema_version() const noexcept;
    const Config& config() const noexcept;
    const std::vector<Layer>& layers() const noexcept;

 private:
    MediaType m_media_type{};
    size_t m_schema_version{};
    Config m_config{};
    std::vector<Layer> m_layers{};
};
}  // namespace cent