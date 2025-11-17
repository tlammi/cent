#pragma once

#include <cent/data/mime.hpp>
#include <cent/error.hpp>
#include <ranges>
#include <rfl.hpp>
#include <rfl/json.hpp>

namespace cent::data {

struct ManifestMsg {
    rfl::Validator<size_t, rfl::EqualTo<2>> schemaVersion{2};
    Mime mediaType{};
    struct Reference {
        Mime mediaType{};
        std::string digest;
        size_t size;
    };
    Reference config;
    std::vector<Reference> layers;

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
    if (res->config.mediaType != mimes::oci_image_config)
        raise(ErrorCode::InvalidArgument, "wrong MIME: {}",
              res->config.mediaType.full());
    return {
        .config =
            {
                .digest = std::move(res->config.digest),
                .size = res->config.size,
            },
        .layers = std::move(res->layers) | std::views::transform([](auto v) {
                      if (v.mediaType != mimes::oci_image_layer)
                          raise(ErrorCode::InvalidArgument, "wrong MIME: {}",
                                v.mediaType.full());
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
