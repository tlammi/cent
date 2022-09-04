#pragma once

#include <string>
#include <string_view>

#include "cent/raise.hpp"

namespace cent {

template <class StringT>
class BasicDigest {
 public:
    BasicDigest(StringT str) : m_str{std::move(str)} {}

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

 private:
    size_t sep() const noexcept { return m_str.find(':'); }
    StringT m_str;
};

using Digest = BasicDigest<std::string>;
using DigestView = BasicDigest<std::string_view>;
}  // namespace cent