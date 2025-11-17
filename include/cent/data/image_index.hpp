#pragma once

#include <cent/data/mime.hpp>
#include <cent/error.hpp>
#include <cent/platform.hpp>
#include <map>
#include <rfl/json.hpp>
#include <string>

namespace cent::data {

struct ImageIdxEntryMsg {
    std::map<std::string, std::string> annotations{};
    // TODO: Special type for digest
    std::string digest{};
    Mime mediaType{};
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
    Platform platform;
    size_t size;
};

struct ImageIdxMsg {
    std::vector<ImageIdxEntry> manifests{};
    Mime mediaType{};
    rfl::Validator<size_t, rfl::EqualTo<2>> schemaVersion{2};
};

using ImageIdx = std::vector<ImageIdxEntry>;

inline ImageIdx parse_image_index(std::string_view data) {
    auto res = rfl::json::read<ImageIdxMsg>(data);
    if (!res) raise(ErrorCode::FormatError, "{}", res.error().what());
    if (res->mediaType != mimes::oci_image_index)
        raise(ErrorCode::FormatError, "wrong MIME: {}", res->mediaType.full());
    return res->manifests;
}

}  // namespace cent::data
namespace rfl {

template <>
class Reflector<cent::data::ImageIdxEntry> {
 public:
    using ReflType = cent::data::ImageIdxEntryMsg;
    static rfl::Result<cent::data::ImageIdxEntry> to(ReflType in) noexcept {
        if (in.mediaType != cent::data::mimes::oci_image_manifest)
            return rfl::error(
                std::format("wrong MIME: {}", in.mediaType.full()));
        return cent::data::ImageIdxEntry{
            .annotations = std::move(in.annotations),
            .digest = std::move(in.digest),
            .platform =
                {
                    .os = std::move(in.platform.os),
                    .arch = std::move(in.platform.architecture),
                },
            .size = std::move(in.size),
        };
    }
};
}  // namespace rfl
