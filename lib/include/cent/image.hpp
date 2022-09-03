#pragma once

#include <string>
#include <string_view>

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
 */
class Image {
 public:
  Image(std::string str);
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
   * This is the same value stored in the constructor.
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

 private:
  static constexpr inline auto NPOS = std::string::npos;
  size_t tag_separator() const noexcept;

  std::string m_str;
};
}  // namespace cent