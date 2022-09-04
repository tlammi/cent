#pragma once

#include <string_view>

#include "cent/enum_arr.hpp"

namespace cent {

enum class MediaKind {
    DockerManifestListV2,
    COUNT_,
};

struct MediaType {
    MediaKind type;
    std::string_view mime;

    static MediaType from_mime(std::string_view mime);
    static MediaType from_kind(MediaKind kind);
};

}  // namespace cent