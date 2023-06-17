#pragma once

#include <fmt/core.h>

#include <string>
#include <string_view>
namespace cent::dm {

template <class Str>
class BasicDigest {
 public:
    BasicDigest() {}
    BasicDigest(Str str) : m_str{std::move(str)} {}

    template <class Str2, class = std::enable_if_t<!std::is_same_v<Str, Str2>>>
    BasicDigest(const BasicDigest<Str2>& other) : m_str{other.str()} {}

    template <class Str2, class = std::enable_if_t<!std::is_same_v<Str, Str2>>>
    BasicDigest(BasicDigest<Str2>&& other) : m_str{std::move(other.str())} {}

    std::string_view algo() const noexcept {
        const auto idx = sep();
        if (idx == Str::npos) return "";
        return std::string_view(m_str).substr(0, idx);
    }

    std::string_view value() const noexcept {
        const auto idx = sep();
        if (idx == Str::npos) return m_str;
        return std::string_view(m_str).substr(idx + 1);
    }

    std::string_view str() const noexcept { return m_str; }

 private:
    size_t sep() const noexcept { return m_str.find(':'); }
    Str m_str{};
};

using Digest = BasicDigest<std::string>;
using DigestView = BasicDigest<std::string_view>;

}  // namespace cent::dm

template <class T>
struct fmt::formatter<cent::dm::BasicDigest<T>>
    : fmt::formatter<std::string_view> {
    auto format(const cent::dm::BasicDigest<T>& d, fmt::format_context& ctx) {
        return fmt::formatter<std::string_view>::format(d.str(), ctx);
    }
};
