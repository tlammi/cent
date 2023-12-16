#include <cent/bits/reg/reference.hpp>
#include <cent/str.hpp>

namespace cent::reg {

std::string_view Reference::repo() const noexcept {
    auto at_idx = digest_idx();
    if (at_idx != std::string::npos)
        return std::string_view(m_s).substr(0, at_idx);

    auto colon_idx = tag_idx();
    if (colon_idx == std::string::npos) return m_s;
    return std::string_view(m_s).substr(0, colon_idx);
}

std::string_view Reference::tag() const noexcept {
    auto at_idx = digest_idx();
    if (at_idx != std::string::npos) return "";
    auto colon_idx = tag_idx();
    if (colon_idx == std::string::npos) return "latest";
    return std::string_view(m_s).substr(colon_idx + 1);
}

std::string_view Reference::digest() const noexcept {
    auto at_idx = m_s.rfind('@');
    if (at_idx == std::string::npos) return "";
    return std::string_view(m_s).substr(at_idx + 1);
}

std::string::size_type Reference::tag_idx() const noexcept {
    auto at_idx = digest_idx();
    if (at_idx != std::string::npos) return std::string::npos;
    auto colon_idx = m_s.rfind(':');
    if (colon_idx == std::string::npos) return std::string::npos;
    auto slash_idx = m_s.rfind('/');
    if (slash_idx == std::string::npos) return colon_idx;
    if (slash_idx > colon_idx) return std::string::npos;
    return colon_idx;
}

std::string::size_type Reference::digest_idx() const noexcept {
    return m_s.rfind('@');
}

}  // namespace cent::reg
