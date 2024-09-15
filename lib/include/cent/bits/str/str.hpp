/**
 * \file
 *
 * */
#pragma once

#include <cent/bits/str/c_str.hpp>
#include <string>
#include <utility>

namespace cent {

/**
 * \brief Sum type for statically or dynamically allocated string
 * */
class Str {
 public:
    constexpr Str() noexcept = default;

    explicit constexpr Str(const char* s) noexcept : m_str(s) {}
    explicit constexpr Str(CStr s) noexcept : m_str(s) {}
    explicit constexpr Str(const std::string& s) noexcept
        : m_str(str_dup<char>(s)), m_alloc(true) {}

    constexpr Str(const Str& other) : m_alloc(other.m_alloc) {
        if (m_alloc) {
            m_str = CStr(str_dup(other.m_str.view()), other.m_str.size());
        } else {
            m_str = other.m_str;
        }
    }

    constexpr Str(Str&& other) noexcept
        : m_str(other.m_str), m_alloc(std::exchange(other.m_alloc, false)) {}

    constexpr Str& operator=(const Str& other) {
        auto tmp = Str(std::move(*this));
        m_alloc = other.m_alloc;
        if (m_alloc) {
            m_str = CStr(str_dup(other.m_str.view()), other.m_str.size());
        } else {
            m_str = other.m_str;
        }
        return *this;
    }

    constexpr Str& operator=(Str&& other) noexcept {
        auto tmp = Str(std::move(*this));
        std::swap(m_str, other.m_str);
        std::swap(m_alloc, other.m_alloc);
        return *this;
    }

    constexpr ~Str() {
        if (m_alloc) delete[] m_str.c_str();
    }

 private:
    CStr m_str{};
    bool m_alloc{false};
};
}  // namespace cent
