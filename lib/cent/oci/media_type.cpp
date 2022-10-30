/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#include "cent/oci/media_type.hpp"

#include "cent/raise.hpp"
#include "cent/util.hpp"

namespace cent::oci {
namespace {

constexpr util::EnumArr<MediaKind, std::string_view> MIME_MAP{
    "application/vnd.docker.image.rootfs.diff.tar.gzip",
    "application/vnd.docker.container.image.v1+json",
    "application/vnd.docker.distribution.manifest.v2+json",
    "application/vnd.docker.distribution.manifest.list.v2+json"};

static_assert(static_cast<size_t>(MediaKind::COUNT_) == MIME_MAP.size(),
              "MediaKind and or MIME_MAP was modified without the other");
}  // namespace

MediaType MediaType::from_mime(std::string_view mime) {
    if (auto maybe_enum = MIME_MAP.find(mime))
        return {*maybe_enum, MIME_MAP[*maybe_enum]};
    raise("Unsupported MIME: '", mime, '\'');
}
MediaType MediaType::from_kind(MediaKind kind) {
    return {kind, MIME_MAP[kind]};
}
}  // namespace cent