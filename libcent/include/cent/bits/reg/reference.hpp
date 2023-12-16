#pragma once

#include <string>

namespace cent::reg {

class Reference {
 public:
    constexpr Reference() = default;
    Reference(std::string s) noexcept : m_s(std::move(s)) {}

    /**
     * \brief Get repository of the reference
     *
     * Returns everything else than the tag and the digest
     * */
    std::string_view repo() const noexcept;
    /**
     * \brief Get tag of the reference
     *
     * Returns tag of the reference, "latest" as the default or an empty string
     * if the reference contains a digest instead.
     * */
    std::string_view tag() const noexcept;

    std::string_view digest() const noexcept;

    const std::string& str() const noexcept { return m_s; }

 private:
    std::string::size_type tag_idx() const noexcept;
    std::string::size_type digest_idx() const noexcept;

    std::string m_s{};
};

}  // namespace cent::reg
