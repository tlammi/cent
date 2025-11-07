#pragma once

#include <string_view>

namespace cent {

class CStr {
 public:
    constexpr CStr(const char* s) noexcept : m_s(s) {}

    constexpr const char* data() const noexcept { return m_s.data(); }
    constexpr const char* c_str() const noexcept { return m_s.data(); }

    template <class S>
    constexpr auto begin(this S&& s) noexcept {
        return std::forward<S>(s).m_s.begin();
    }

    template <class S>
    constexpr auto end(this S&& s) noexcept {
        return std::forward<S>(s).m_s.end();
    }

 private:
    std::string_view m_s;
};
}  // namespace cent
