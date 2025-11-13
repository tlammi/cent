#pragma once

#include <cent/data/mime.hpp>
#include <cent/error.hpp>
#include <ranges>
#include <rfl.hpp>
#include <rfl/json.hpp>

namespace cent::data {

#if 0
{
  "schemaVersion": 2,
  "mediaType": "application/vnd.oci.image.manifest.v1+json",
  "config": {
    "mediaType": "application/vnd.oci.image.config.v1+json",
    "digest": "sha256:706db57fb2063f39f69632c5b5c9c439633fda35110e65587c5d85553fd1cc38",
    "size": 581
  },
  "layers": [
    {
      "mediaType": "application/vnd.oci.image.layer.v1.tar+gzip",
      "digest": "sha256:2d35ebdb57d9971fea0cac1582aa78935adf8058b2cc32db163c98822e5dfa1b",
      "size": 3802452
    }
  ],
  "annotations": {
    "com.docker.official-images.bashbrew.arch": "amd64",
    "org.opencontainers.image.base.name": "scratch",
    "org.opencontainers.image.created": "2025-10-08T11:04:56Z",
    "org.opencontainers.image.revision": "4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d",
    "org.opencontainers.image.source": "https://github.com/alpinelinux/docker-alpine.git#4dc13cbc7caffe03c98aa99f28e27c2fb6f7e74d:x86_64",
    "org.opencontainers.image.url": "https://hub.docker.com/_/alpine",
    "org.opencontainers.image.version": "3.22.2"
  }
}
#endif
struct ManifestMsg {
    rfl::Validator<size_t, rfl::EqualTo<2>> schemaVersion{2};
    mime_value<Mime::OciImageManifest>::literal mediaType{};
    struct Config {
        mime_value<Mime::OciImageConfig>::literal mediaType{};
        std::string digest;
        size_t size;
    };
    Config config;

    struct Layer {
        mime_value<Mime::OciImageLayer>::literal mediaType{};
        std::string digest;
        size_t size;
    };

    std::vector<Layer> layers;

    std::map<std::string, std::string> annotations{};
};

struct Manifest {
    struct Reference {
        std::string digest;
        size_t size;
    };
    Reference config;
    std::vector<Reference> layers{};
    std::map<std::string, std::string> annotations{};
};

inline Manifest parse_manifest(std::string_view str) {
    auto res = rfl::json::read<ManifestMsg>(str);
    if (!res) raise(ErrorCode::FormatError, "{}", res.error().what());
    return {
        .config =
            {
                .digest = std::move(res->config.digest),
                .size = res->config.size,
            },
        .layers = std::move(res->layers) | std::views::transform([](auto v) {
                      return Manifest::Reference{
                          .digest = std::move(v.digest),
                          .size = v.size,
                      };
                  }) |
                  std::ranges::to<std::vector>(),
        .annotations = std::move(res->annotations),
    };
}

}  // namespace cent::data
