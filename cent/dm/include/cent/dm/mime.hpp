/**
 * \file mime.hpp
 *
 * MIME types for working with containers
 * */
#pragma once

#include <cent/core.hpp>

namespace cent::dm {

enum class MimeType {
    ManifestList,
    COUNT_,
};
namespace mime_detail {
using namespace cent::literals;
constexpr EnumArr<MimeType, StaticStr> mime_map_docker =
    make_enum_arr<MimeType, StaticStr>(
        MimeType::ManifestList,
        "application/vnd.docker.distribution.manifest.list.v2+json"_ss);

constexpr EnumArr<MimeType, StaticStr> mime_map_oci =
    make_enum_arr<MimeType, StaticStr>(MimeType::ManifestList,
                                       "manifest.list.v2+json"_ss);
}  // namespace mime_detail

constexpr StaticStr docker_mime(MimeType type) {
    return mime_detail::mime_map_docker[type];
}

constexpr StaticStr oci_mime(MimeType type) {
    return mime_detail::mime_map_oci[type];
}

constexpr StaticStr mime(MimeType type, bool is_docker) noexcept {
    if (is_docker) return docker_mime(type);
    return oci_mime(type);
}

constexpr std::optional<MimeType> docker_mime_from_str(
    std::string_view s) noexcept {
    return mime_detail::mime_map_docker.find(s);
}

constexpr std::optional<MimeType> oci_mime_from_str(
    std::string_view s) noexcept {
    return mime_detail::mime_map_oci.find(s);
}

}  // namespace cent::dm
