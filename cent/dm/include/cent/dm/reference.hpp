#pragma once

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
    ViewT tag() const noexcept;

    /**
     * Return the unmodified string.
     *
     * This is the same value stored in the object e.g. via constructor.
     */
    ViewT str() const noexcept;

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
    ViewT registry() const noexcept;

    /**
     * Image name without registry and tag parts
     *
     * Note that this expects a domain to be present.
     */
    ViewT name() const noexcept;

    /**
     * Everything except the tag
     */
    ViewT repo() const noexcept;

    // TODO: Change to DigestView
    ViewT digest() const noexcept;

 private:
    StrT m_s{};
};

using Reference = BasicReference<std::string, std::string_view>;
using ReferenceView = BasicReference<std::string_view, std::string_view>;
}  // namespace cent::dm
