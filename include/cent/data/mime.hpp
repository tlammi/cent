#pragma once

#include <cent/static_str.hpp>
#include <cent/util/enum_arr.hpp>
#include <rfl.hpp>

namespace cent::data {

enum class Mime {
    OciImageIndex,
    OciImageManifest,
};

template <auto>
struct mime_value;

template <>
struct mime_value<Mime::OciImageIndex> {
    using literal = rfl::Literal<"application/vnd.oci.image.index.v1+json">;
};

template <>
struct mime_value<Mime::OciImageManifest> {
    using literal = rfl::Literal<"application/vnd.oci.image.manifest.v1+json">;
};

namespace detail {
consteval auto mk_mime_arr() noexcept {
    using P = std::pair<Mime, StaticStr>;
    using enum Mime;
    return util::make_array<StaticStr, Mime>(
        P{OciImageIndex, "application/vnd.oci.image.index.v1+json"},
        P{OciImageManifest, "application/vnd.oci.image.manifest.v1+json"});
}
}  // namespace detail

constexpr auto MIME_VALUES = detail::mk_mime_arr();

}  // namespace cent::data
template <>
class rfl::Reflector<cent::data::Mime> {
    using ReflType = std::string;
    static rfl::Result<cent::data::Mime> to(const std::string& str) noexcept {
        auto res = cent::data::MIME_VALUES.index_of(str);
        if (res) return *res;
        return rfl::error(std::format("Unsupported mime '{}'", str));
    }

    static std::string from(cent::data::Mime mime) noexcept {
        return std::string(cent::data::MIME_VALUES[mime].string_view());
    }
};
