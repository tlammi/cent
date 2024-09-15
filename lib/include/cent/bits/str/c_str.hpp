/**
 * \file
 * */
#pragma once

#include <cent/bits/str/util.hpp>
#include <cstddef>
#include <string_view>

namespace cent {

/**
 * \brief String view with ensured null termination
 * */
template <class CharT>
class BasicCStr {
 public:
    /**
     * \brief Construct an empty string
     * */
    constexpr BasicCStr() noexcept = default;

    constexpr BasicCStr(const CharT* s) noexcept
        : m_ptr(s), m_len(str_len(s)) {}
    constexpr BasicCStr(const CharT* s, size_t len) noexcept
        : m_ptr(s), m_len(len) {}

    constexpr void remove_prefix(size_t count) noexcept {
        m_ptr += count;
        m_len -= count;
    }

    operator const CharT*() const noexcept { return m_ptr; }

    constexpr const CharT* c_str() const noexcept { return m_ptr; }
    constexpr std::string_view view() const noexcept { return {m_ptr, m_len}; }

    constexpr size_t length() const noexcept { return m_len; }
    constexpr size_t size() const noexcept { return m_len; }

 private:
    const CharT* m_ptr{""};
    size_t m_len{};
};

using CStr = BasicCStr<char>;

}  // namespace cent
