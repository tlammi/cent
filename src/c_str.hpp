#pragma once

#include <string_view>

namespace cent {

struct null_terminated_t {};

constexpr auto null_terminated = null_terminated_t{};

class CStr {
 public:
    constexpr CStr() noexcept = default;
    constexpr CStr(const char* s) noexcept : m_s(s) {}
    constexpr CStr(std::string_view s, null_terminated_t) noexcept : m_s(s) {}

    constexpr const char* data() const noexcept { return m_s.data(); }
    constexpr const char* c_str() const noexcept { return m_s.data(); }

    constexpr std::string_view view() const noexcept { return m_s; }

    constexpr size_t size() const noexcept { return m_s.size(); }

    template <class S>
    constexpr auto begin(this S&& s) noexcept {
        return std::forward<S>(s).m_s.begin();
    }

    template <class S>
    constexpr auto end(this S&& s) noexcept {
        return std::forward<S>(s).m_s.end();
    }

    constexpr bool starts_with(std::string_view s) const noexcept {
        return m_s.starts_with(s);
    }

    constexpr char operator[](size_t idx) const noexcept { return m_s[idx]; }

    constexpr auto operator<=>(const CStr&) const noexcept = default;

 private:
    std::string_view m_s{""};
};
}  // namespace cent
