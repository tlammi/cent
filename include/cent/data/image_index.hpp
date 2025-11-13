#pragma once

#include <cent/data/mime.hpp>
#include <cent/error.hpp>
#include <map>
#include <rfl/json.hpp>
#include <string>

namespace cent::data {
namespace msgs {}  // namespace msgs

struct ImageIdxEntryMsg {
    std::map<std::string, std::string> annotations{};
    // TODO: Special type for digest
    std::string digest{};
    mime_value<Mime::OciImageManifest>::literal mediaType{};
    struct {
        std::string architecture;
        std::string os;
    } platform;
    size_t size;
};

struct ImageIdxEntry {
    std::map<std::string, std::string> annotations{};
    // TODO: Special type for digest
    std::string digest{};
    struct {
        std::string architecture;
        std::string os;
    } platform;
    size_t size;
};

struct ImageIdxMsg {
    std::vector<ImageIdxEntry> manifests{};
    mime_value<Mime::OciImageIndex>::literal mediaType{};
    rfl::Validator<size_t, rfl::EqualTo<2>> schemaVersion{2};
};

using ImageIdx = std::vector<ImageIdxEntry>;

inline ImageIdx parse_image_index(std::string_view data) {
    auto res = rfl::json::read<ImageIdxMsg>(data);
    if (!res) raise(ErrorCode::FormatError, "{}", res.error().what());
    return res->manifests;
}

}  // namespace cent::data
namespace rfl {

template <>
class Reflector<cent::data::ImageIdxEntry> {
 public:
    using ReflType = cent::data::ImageIdxEntryMsg;
    static cent::data::ImageIdxEntry to(ReflType in) noexcept {
        return {
            .annotations = std::move(in.annotations),
            .digest = std::move(in.digest),
            .platform =
                {
                    .architecture = std::move(in.platform.architecture),
                    .os = std::move(in.platform.os),
                },
            .size = std::move(in.size),
        };
    }
};
}  // namespace rfl
