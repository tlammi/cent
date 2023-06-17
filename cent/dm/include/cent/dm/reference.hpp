#pragma once

#include <fmt/core.h>

#include <cent/dm/digest.hpp>
#include <string>
#include <string_view>
#include <type_traits>

namespace cent::dm {
namespace ref_detail {
/// Find index where "tag" part of the URI starts
size_t tag_sep(std::string_view s);
/// Find index where "digest" part of the URI starts
size_t digest_sep(std::string_view s);
/// Find indes where "reference" stops, i.e. where tag or digest starts
size_t ref_sep(std::string_view s);
}  // namespace ref_detail

template <class StrT, class ViewT>
class BasicReference {
    static constexpr auto NPOS = ViewT::npos;

 public:
    BasicReference() = default;
    explicit BasicReference(StrT s) : m_s{std::move(s)} {}

    /**
     * Access the tag in the image
     *
     * E.g.
     * - "foo:bar" -> "bar"
     * - "foo" -> "latest"
     */
    ViewT tag() const noexcept {
        auto tag_idx = ref_detail::tag_sep(m_s);
        if (tag_idx != NPOS) return ViewT{m_s}.substr(tag_idx + 1);
        auto dig_idx = ref_detail::digest_sep(m_s);
        if (dig_idx == NPOS) return "latest";
        return "";
    }

    /**
     * Return the unmodified string.
     *
     * This is the same value stored in the object e.g. via constructor.
     */
    ViewT str() const noexcept { return m_s; }

    /**
     * Get the registry name from the image
     *
     * Parses up to the first '/' and returns that. If no registry is stored in
     * the image, an empty string is returned.
     * E.g.
     * - "foo" -> ""
     * - "localhost/bar" -> "localhost"
     * - "localhost:5000/bar" -> "localhost:5000"
     */
    ViewT registry() const noexcept {
        const auto slash = m_s.find('/');
        if (slash == NPOS) return "";
        return ViewT{m_s}.substr(0, slash);
    }

    /**
     * Image name without registry and tag parts
     *
     * Note that this expects a domain to be present.
     */
    ViewT name() const noexcept {
        ViewT view{m_s};
        const auto tag_sep = ref_detail::ref_sep(view);
        if (tag_sep != NPOS) view = view.substr(0, tag_sep);
        const auto slash_sep = view.find('/');
        if (slash_sep != NPOS) view = view.substr(slash_sep + 1);
        return view;
    }

    /**
     * Everything except the tag
     */
    ViewT repo() const noexcept {
        auto idx = ref_detail::ref_sep(m_s);
        if (idx != NPOS) return ViewT{m_s}.substr(0, idx);
        return ViewT{m_s};
    }

    /**
     * Digest field
     * */
    DigestView digest() const noexcept {
        const auto at_idx = ref_detail::digest_sep(m_s);
        if (at_idx == NPOS) return DigestView{""};
        return std::string_view(m_s).substr(at_idx + 1);
    }

 private:
    StrT m_s{};
};

using Reference = BasicReference<std::string, std::string_view>;
using ReferenceView = BasicReference<std::string_view, std::string_view>;

}  // namespace cent::dm

template <class T, class V>
struct fmt::formatter<cent::dm::BasicReference<T, V>>
    : fmt::formatter<std::string_view> {
    auto format(const cent::dm::BasicReference<T, V>& r,
                fmt::format_context& ctx) {
        return fmt::formatter<std::string_view>::format(r.str(), ctx);
    }
};

