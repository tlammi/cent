#include "cent/image.hpp"
namespace cent {

Image::Image(std::string str) : m_str{std::move(str)} {}
Image& Image::operator=(std::string str) {
    m_str = std::move(str);
    return *this;
}

std::string_view Image::tag() const noexcept {
    const auto at_idx = digest_separator();
    const auto colon_idx = tag_separator();
    if (at_idx == NPOS && colon_idx == NPOS) return "latest";
    if (at_idx == NPOS) return std::string_view(m_str).substr(colon_idx + 1);
    return "";
}

std::string_view Image::registry() const noexcept {
    const auto slash = m_str.find('/');
    if (slash == NPOS) return "";
    return std::string_view(m_str).substr(0, slash);
}
std::string_view Image::name() const noexcept {
    std::string_view view = m_str;
    const auto tag_sep = ref_separator();
    if (tag_sep != NPOS) view = view.substr(0, tag_sep);
    const auto slash_sep = view.find('/');
    if (slash_sep != NPOS) view = view.substr(slash_sep + 1);
    return view;
}

std::string_view Image::repo() const noexcept {
    const auto tag_sep = ref_separator();
    std::string_view view = m_str;
    if (tag_sep != NPOS) return view = view.substr(0, tag_sep);
    return view;
}

const std::string& Image::str() const noexcept { return m_str; }

DigestView Image::digest() const noexcept {
    const auto at_idx = digest_separator();
    if (at_idx == NPOS) return DigestView{""};
    return std::string_view(m_str).substr(at_idx + 1);
}

size_t Image::tag_separator() const noexcept {
    const auto colon = m_str.rfind(':');
    if (colon == NPOS) return NPOS;
    const auto slash = m_str.rfind('/');
    // colon before slash -> found port separator
    if (slash != NPOS && slash > colon) return NPOS;
    return colon;
}

size_t Image::ref_separator() const noexcept {
    auto at = digest_separator();
    if (at != NPOS) return at;
    return tag_separator();
}

size_t Image::digest_separator() const noexcept { return m_str.rfind('@'); }

}  // namespace cent