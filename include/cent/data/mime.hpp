#pragma once

#include <cent/error.hpp>
#include <cent/static_str.hpp>
#include <cent/util/enum_arr.hpp>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <string_view>

namespace cent::data {

template <class T>
class BasicMime {
    static constexpr auto npos = T::npos;
    using size_type = T::size_type;

 public:
    constexpr BasicMime() noexcept = default;
    constexpr BasicMime(T val, size_type slash, size_type plus) noexcept
        : m_v(std::move(val)), m_slash(slash), m_plus(plus) {
        assert(m_slash != npos);
        assert(m_slash < m_v.size());
        if (m_plus == npos) {
            assert(m_plus > m_slash);
            assert(m_plus < m_v.size());
        }
    }
    constexpr explicit BasicMime(T s)
        : m_v(std::move(s)), m_slash(m_v.find('/')) {
        if (m_slash == npos)
            raise(ErrorCode::InvalidArgument, "Invalid MIME '{}'", m_v);
        m_plus = m_v.find('+', m_slash + 1);
    }

    constexpr BasicMime(const BasicMime&) = default;
    constexpr BasicMime& operator=(const BasicMime&) = default;

    constexpr BasicMime(BasicMime&&) = default;
    constexpr BasicMime& operator=(BasicMime&&) = default;

    constexpr ~BasicMime() = default;

    template <class S>
    constexpr decltype(auto) full(this S&& self) noexcept {
        return std::forward<S>(self).m_v;
    }

    constexpr std::string_view string_view() const noexcept {
        return std::string_view(m_v);
    }

    constexpr std::string_view type() const noexcept {
        return string_view().substr(m_slash);
    }

    constexpr std::string_view subtype() const noexcept {
        const size_type len = m_plus == npos ? npos : m_plus - m_slash - 1;
        return string_view().substr(m_slash, len);
    }

    constexpr std::string_view suffix() const noexcept {
        return m_plus == npos ? std::string_view()
                              : string_view().substr(m_plus);
    }

    template <class T2>
    constexpr bool operator==(const BasicMime<T2>& other) const noexcept {
        return string_view() == other.string_view();
    }

    template <class T2>
    constexpr bool operator!=(const BasicMime<T2>& other) const noexcept {
        return string_view() != other.string_view();
    }

    template <class T2>
    constexpr auto operator<=>(const BasicMime<T2>& other) const noexcept {
        return string_view() <=> other.string_view();
    }

    constexpr bool operator==(std::string_view s) const noexcept {
        return m_v == s;
    }

 private:
    T m_v;
    size_type m_slash;
    size_type m_plus;
};

using Mime = BasicMime<std::string>;
using MimeView = BasicMime<std::string_view>;

constexpr std::optional<Mime> mime(std::string val) noexcept {
    auto slash = val.find('/');
    if (slash == std::string_view::npos) return std::nullopt;
    auto plus = val.find('+', slash + 1);
    return Mime(std::move(val), slash, plus);
}

namespace literals {
consteval auto operator""_mime(const char* ptr, std::size_t len) {
    return MimeView(std::string_view(ptr, len));
}
}  // namespace literals
namespace mimes {
using namespace cent::data::literals;
constexpr auto oci_image_index = "application/vnd.oci.image.index.v1+json"_mime;
constexpr auto oci_image_manifest =
    "application/vnd.oci.image.manifest.v1+json"_mime;
constexpr auto oci_image_config =
    "application/vnd.oci.image.config.v1+json"_mime;
constexpr auto oci_image_layer =
    "application/vnd.oci.image.layer.v1.tar+gzip"_mime;

}  // namespace mimes

}  // namespace cent::data

template <class T>
class rfl::Reflector<cent::data::BasicMime<T>> {
 public:
    using ReflType = T;
    static rfl::Result<cent::data::Mime> to(ReflType str) noexcept {
        auto res = cent::data::mime(std::move(str));
        if (res) return *res;
        return rfl::error(std::format("Unsupported mime '{}'", str));
    }

    static ReflType from(cent::data::Mime mime) noexcept {
        return std::move(mime).full();
    }
};
