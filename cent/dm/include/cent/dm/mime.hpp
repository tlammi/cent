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
};

constexpr StaticStr docker_mime(MimeType type) {
    using enum MimeType;
    switch (type) {
        case ManifestList:
            return "application/"
                   "vnd.docker.distribution.manifest.list.v2+json";
    }
    ::std::terminate();
}

constexpr StaticStr oci_mime(MimeType type) {
    using enum MimeType;
    switch (type) {
        case MimeType::ManifestList: return "manifest.list.v2+json";
    }
    ::std::terminate();
}

constexpr StaticStr mime(MimeType type, bool is_docker) noexcept {
    if (is_docker) return docker_mime(type);
    return oci_mime(type);
}
}  // namespace cent::dm
