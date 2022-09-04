#include "cent/media_type.hpp"

#include "cent/raise.hpp"

namespace cent {
namespace {

constexpr EnumArr<MediaKind, std::string_view> MIME_MAP{
    "application/vnd.docker.distribution.manifest.list.v2+json"};

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