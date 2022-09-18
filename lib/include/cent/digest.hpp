/* SPDX-License-Identifier:  GPL-3.0-or-later */
/* Copyright (C) 2022 Toni Lammi */
#pragma once

#include <string>
#include <string_view>

#include "cent/raise.hpp"

namespace cent {

template <class StringT>
class BasicDigest {
 public:
    BasicDigest() {}
    BasicDigest(StringT str) : m_str{std::move(str)} {}

    template <class StringT2,
              class = std::enable_if_t<!std::is_same_v<StringT, StringT2>>>
    BasicDigest(const BasicDigest<StringT2>& other) : m_str{other.str()} {}

    template <class StringT2,
              class = std::enable_if_t<!std::is_same_v<StringT, StringT2>>>
    BasicDigest(BasicDigest<StringT2>&& other)
        : m_str{std::move(other.str())} {}

    std::string_view algo() const noexcept {
        const auto idx = sep();
        if (idx == StringT::npos) return "";
        return std::string_view(m_str).substr(0, idx);
    }

    std::string_view value() const noexcept {
        const auto idx = sep();
        if (idx == StringT::npos) return m_str;
        return std::string_view(m_str).substr(idx + 1);
    }

    std::string_view str() const noexcept { return m_str; }

 private:
    size_t sep() const noexcept { return m_str.find(':'); }
    StringT m_str{};
};

using Digest = BasicDigest<std::string>;
using DigestView = BasicDigest<std::string_view>;

template <class S>
std::ostream& operator<<(std::ostream& os, const BasicDigest<S>& d) {
    os << d.str();
    return os;
}
}  // namespace cent