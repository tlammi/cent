#include "cent/image.hpp"
namespace cent {

Image::Image(std::string str) : m_str{std::move(str)} {}
Image& Image::operator=(std::string str) {
  m_str = std::move(str);
  return *this;
}

std::string_view Image::tag() const noexcept {
  const auto sep = tag_separator();
  if (sep == NPOS) return "latest";
  return std::string_view(m_str).substr(sep + 1);
}

std::string_view Image::registry() const noexcept {
  const auto slash = m_str.find('/');
  if (slash == NPOS) return "";
  return std::string_view(m_str).substr(0, slash);
}
std::string_view Image::name() const noexcept {
  std::string_view view = m_str;
  const auto tag_sep = tag_separator();
  if (tag_sep != NPOS) view = view.substr(0, tag_sep);
  const auto slash_sep = view.find('/');
  if (slash_sep != NPOS) view = view.substr(slash_sep + 1);
  return view;
}

std::string_view Image::repo() const noexcept {
  const auto tag_sep = tag_separator();
  std::string_view view = m_str;
  if (tag_sep != NPOS) return view = view.substr(0, tag_sep);
  return view;
}

const std::string& Image::str() const noexcept { return m_str; }

size_t Image::tag_separator() const noexcept {
  const auto colon = m_str.rfind(':');
  if (colon == NPOS) return NPOS;
  const auto slash = m_str.rfind('/');
  // colon before slash -> found port separator
  if (slash != NPOS && slash > colon) return NPOS;
  return colon;
}
}  // namespace cent