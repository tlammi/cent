#pragma once

#include <string>
#include <string_view>

#include "cent/digest.hpp"

namespace cent {

/**
 * Image reference name
 *
 * E.g.
 * - docker.io/ubuntu:20.04
 * - localhost:5000/foo
 * - localhost/foo:bar
 * - bar:baz
 * - foo
 *
 * TODO: Rename as it can reference various things, like manifests or blobs.
 */
class Image {
 public:
    /**
     * Image from a reference
     *
     * \param str E.g. "docker.io/ubuntu:20.40"
     */
    Image(std::string str);

    /// Assign a new value to the str
    Image& operator=(std::string str);

    Image(const Image&) = default;
    Image& operator=(const Image&) = default;

    Image(Image&&) noexcept = default;
    Image& operator=(Image&&) noexcept = default;

    /**
     * Access the tag in the image
     *
     * E.g.
     * - "foo:bar" -> "bar"
     * - "foo" -> "latest"
     */
    std::string_view tag() const noexcept;

    /**
     * Return the unmodified string.
     *
     * This is the same value stored in the object e.g. via constructor.
     */
    const std::string& str() const noexcept;

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
    std::string_view registry() const noexcept;

    /**
     * Image name without registry and tag parts
     *
     * Note that this expects a domain to be present.
     */
    std::string_view name() const noexcept;

    /**
     * Everything except the tag
     */
    std::string_view repo() const noexcept;

    DigestView digest() const noexcept;

 private:
    static constexpr inline auto NPOS = std::string::npos;
    size_t tag_separator() const noexcept;
    size_t digest_separator() const noexcept;
    size_t ref_separator() const noexcept;

    std::string m_str;
};
}  // namespace cent