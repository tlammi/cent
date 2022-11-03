/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <string_view>
#include <vector>

namespace cent::oci {

enum class MediaKind {
    DockerImageRootfsDiffTarGz,
    DockerContainerImageV1,
    DockerManifestV2,
    DockerManifestListV2,
    COUNT_,
};

/**
 * Wrapper for MIME types
 */
struct MediaType {
    MediaKind type;  ///< Enumeration
    std::string_view
        mime;  ///< String representation. This is statically allocated

    /**
     * Create MediaType from MIME string
     */
    static MediaType from_mime(std::string_view mime);

    /**
     * Create MediaType from enumeration
     */
    static MediaType from_kind(MediaKind kind);

    constexpr bool operator==(const MediaType& other) const noexcept {
        return type == other.type;
    }
    constexpr bool operator!=(const MediaType& other) const noexcept {
        return type != other.type;
    }
};

std::vector<MediaType> all_media_types();

inline std::string_view media_mime(MediaKind kind) {
    return MediaType::from_kind(kind).mime;
}

}  // namespace cent::oci