#pragma once

#include <cstddef>
#include <string_view>
namespace cent::str {

/**
 * \brief String view type with null termination
 * */
class CStr {
 public:
    constexpr CStr() noexcept = default;
    constexpr CStr(std::nullptr_t) = delete;
    template <size_t S>
    constexpr CStr(const char (&arr)[S]) : m_str(arr) {}
    constexpr CStr(const char* str) : m_str(str) {}

    constexpr CStr(const CStr&) noexcept = default;
    constexpr CStr(CStr&&) noexcept = default;

    constexpr bool operator==(const CStr& other) const noexcept {
        return m_str == other.m_str;
    }

    constexpr bool operator==(const char* other) const noexcept {
        return m_str == other;
    }

    constexpr std::string_view view() const noexcept { return m_str; }
    constexpr const char* c_str() const noexcept { return m_str.data(); }

    constexpr operator const char*() const noexcept { return m_str.data(); }

    constexpr bool empty() const noexcept { return m_str.empty(); }
    constexpr size_t size() const noexcept { return m_str.size(); }
    constexpr size_t length() const noexcept { return m_str.size(); }

 private:
    std::string_view m_str{};
};
}  // namespace cent::str
