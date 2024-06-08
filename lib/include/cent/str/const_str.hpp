#pragma once

#include <cent/str/c_str.hpp>
#include <cent/str/static_str.hpp>
#include <cstring>
#include <string>
#include <utility>

namespace cent::str {

namespace const_str_detail {
constexpr const char* strdup(CStr s) {
    auto* buf = new char[s.size() + 1];
    std::memcpy(buf, s.c_str(), s.size() + 1);
    return buf;
}
}  // namespace const_str_detail

/**
 * \brief Constant string type
 *
 *
 * This points to either dynamically or statically allocated string.
 * The behavior is useful with constant expressions.
 *
 * */
class ConstStr {
 public:
    constexpr ConstStr() noexcept = default;
    constexpr ConstStr(StaticStr s)
        : m_str(s.c_str() ? s.c_str() : ""), m_alloc(false) {}

    constexpr ConstStr(const std::string& s)
        : m_str(const_str_detail::strdup(s.c_str())), m_alloc(true) {}

    constexpr ConstStr(const ConstStr& other)
        : m_str(const_str_detail::strdup(other.m_str.c_str())), m_alloc(true) {}

    constexpr ConstStr(ConstStr&& other) noexcept
        : m_str(std::move(other.m_str)),
          m_alloc(std::exchange(other.m_alloc, false)) {}

    constexpr ~ConstStr() {
        if (m_alloc) delete[] m_str.c_str();
    }

    constexpr bool operator==(const ConstStr& other) const noexcept {
        return m_str == other.m_str;
    }

    constexpr bool operator==(CStr other) const noexcept {
        return m_str == other;
    }

    constexpr bool dynamically_allocated() const noexcept { return m_alloc; }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_str.empty();
    }
    [[nodiscard]] constexpr CStr view() const noexcept { return m_str; }

 private:
    CStr m_str{};
    bool m_alloc{false};
};
}  // namespace cent::str
