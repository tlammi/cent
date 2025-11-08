/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <string_view>

#include "cent/enum_arr.hpp"

namespace cent {

enum class MediaKind {
    DockerImageRootfsDiffTarGz,
    DockerContainerImageV1,
    DockerManifestV2,
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