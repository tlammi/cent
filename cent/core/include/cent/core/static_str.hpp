#pragma once

#include <cstddef>
#include <string_view>
namespace cent {

class StaticStr {
 public:
    consteval StaticStr() = default;
    template <size_t S>
    explicit consteval StaticStr(const char (&str)[S]) : m_s{str}, m_size{S} {}

    consteval StaticStr(const char* str, size_t len) : m_s{str}, m_size{len} {}

    consteval StaticStr(const char* str)
        : m_s{str}, m_size{std::string_view(str).size()} {}

    constexpr StaticStr(const StaticStr&) = default;
    constexpr StaticStr& operator=(const StaticStr&) = default;
    constexpr StaticStr(StaticStr&&) = default;
    constexpr StaticStr& operator=(StaticStr&&) = default;

    consteval StaticStr& operator=(std::string_view other) {
        m_s = other.data();
        m_size = other.size();
        return *this;
    }
    template <size_t S>
    consteval StaticStr& operator=(const char (&other)[S]) {
        m_s = other;
        m_size = S;
        return *this;
    }

    constexpr std::string_view view() const noexcept { return {m_s, m_size}; }
    constexpr operator std::string_view() const noexcept { return view(); }

    constexpr const char* c_str() const noexcept { return m_s; }
    constexpr const char* data() const noexcept { return m_s; }
    constexpr operator const char*() const noexcept { return m_s; }

 private:
    const char* m_s{nullptr};
    size_t m_size{0};
};

namespace literals {

consteval StaticStr operator""_ss(const char* str, std::size_t len) {
    return StaticStr{str, len};
}
}  // namespace literals
}  // namespace cent
