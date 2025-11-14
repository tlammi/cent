#pragma once

#include <cent/data/mime.hpp>
#include <cent/error.hpp>
#include <ranges>
#include <rfl.hpp>
#include <rfl/json.hpp>

namespace cent::data {

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
